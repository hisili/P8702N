#if defined(CONFIG_BCM_KF_ARM_BCM963XX)
/*
<:copyright-BRCM:2013:DUAL/GPL:standard

   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
*/

/* CPU Frequency scaling support for BCM63xx ARM series */

#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/io.h>
#include <linux/cpufreq.h>
#include <linux/suspend.h>

#include <bcm_map_part.h>
#include <asm/cpu.h>

/* frequency in units of kHz */
struct cpufreq_frequency_table bcm63xx_freq_normal_table[] = {
#if defined CONFIG_BCM963138
/* support divisors of 2GHz */
	{10,  200000},
	{8,   250000},
	{6,   333333},
	{5,   400000},
	{4,   500000},
	{3,   666667},
	{2,  1000000},
#elif defined CONFIG_BCM963148
/* support divisors of 3GHz */
	{16,  187500},
	{8,   375000},
	{4,   750000},
	{2,  1500000},
#endif
	{0, CPUFREQ_TABLE_END},
};

/* frequency is in the unit of kHz */
struct cpufreq_frequency_table bcm63xx_freq_extended_table[] = {
#if defined CONFIG_BCM963138
	{10,  200000},
	{9,   222222},
	{8,   250000},
	{7,   285714},
	{6,   333333},
	{5,   400000},
	{4,   500000},
	{3,   666667},
	{2,  1000000},
#elif defined CONFIG_BCM963148
	{32,   93750},
//	{30,  100000},
//	{24,  125000},
//	{20,  150000},
	{16,  187500},
//	{15,  200000},
//	{12,  250000},
//	{10,  300000},
	{8,   375000},
//	{6,   500000},
//	{5,   600000},
	{4,   750000},
//	{3,  1000000},
	{2,  1500000},
#endif
	{0, CPUFREQ_TABLE_END},
};

struct cpufreq_frequency_table *bcm63xx_freq_table = bcm63xx_freq_normal_table;

int bcm63xx_cpufreq_verify_speed(struct cpufreq_policy *policy)
{
	return cpufreq_frequency_table_verify(policy, bcm63xx_freq_table);
}

unsigned int bcm63xx_cpufreq_getspeed(unsigned int cpu)
{
	struct clk *arm_clk;
	unsigned long arm_freq;

	arm_clk = clk_get_sys("cpu", "arm_pclk");
	BUG_ON(IS_ERR_OR_NULL(arm_clk));
	arm_freq = clk_get_rate(arm_clk);
	BUG_ON(!arm_freq);

	return (arm_freq / 1000);
}

/*
 * loops_per_jiffy is not updated on SMP systems in cpufreq driver.
 * Update the per-CPU loops_per_jiffy value on frequency transition.
 * And don't forget to adjust the global one.
 */
static void adjust_jiffies(cpumask_var_t cpus, struct cpufreq_freqs *freqs)
{
#ifdef CONFIG_SMP
	extern unsigned long loops_per_jiffy;
	static struct lpj_info {
		unsigned long ref;
		unsigned int  freq;
	} global_lpj_ref;
	unsigned cpu;

	if (freqs->flags & CPUFREQ_CONST_LOOPS)
		return;
	if (!global_lpj_ref.freq) {
		global_lpj_ref.ref = loops_per_jiffy;
		global_lpj_ref.freq = freqs->old;
	}

	loops_per_jiffy =
		cpufreq_scale(global_lpj_ref.ref, global_lpj_ref.freq, freqs->new);

	for_each_cpu(cpu, cpus) {
		per_cpu(cpu_data, cpu).loops_per_jiffy = loops_per_jiffy;
	}
#endif
}

static int bcm63xx_cpufreq_target(struct cpufreq_policy *policy,
		unsigned int target_freq,
		unsigned int relation)
{
	struct cpufreq_freqs freqs;
	unsigned int index, old_index;
	int ret = 0;

	freqs.old = policy->cur;

	if (cpufreq_frequency_table_target(policy, bcm63xx_freq_table,
				freqs.old, relation, &old_index))
		return -EINVAL;

	if (cpufreq_frequency_table_target(policy, bcm63xx_freq_table,
				target_freq, relation, &index))
		return -EINVAL;

	freqs.new = bcm63xx_freq_table[index].frequency;
	freqs.cpu = policy->cpu;

	cpufreq_notify_transition(&freqs, CPUFREQ_PRECHANGE);

	if (freqs.new != freqs.old) {
		struct clk *arm_clk;

		arm_clk = clk_get_sys("cpu", "arm_pclk");
		BUG_ON(IS_ERR_OR_NULL(arm_clk));

		ret = clk_set_rate(arm_clk, freqs.new * 1000);
		if (ret != 0)
			freqs.new = freqs.old;
	}

	adjust_jiffies(policy->cpus, &freqs);

	cpufreq_notify_transition(&freqs, CPUFREQ_POSTCHANGE);
	
	return ret;
}

static ssize_t store_set_freq_table(struct cpufreq_policy *policy,
		const char *buf, size_t count)
{
	unsigned int ret;
	char str_freqtable[16];
	struct cpufreq_policy new_policy;

	ret = sscanf(buf, "%15s", str_freqtable);
	if (ret != 1)
		return -EINVAL;

	if (!strnicmp(str_freqtable, "normal", 16)) {
		if (bcm63xx_freq_table == bcm63xx_freq_normal_table)
			return count;
		bcm63xx_freq_table = bcm63xx_freq_normal_table;
	} else if (!strnicmp(str_freqtable, "extended", 16)) {
		if (bcm63xx_freq_table == bcm63xx_freq_extended_table)
			return count;
		bcm63xx_freq_table = bcm63xx_freq_extended_table;
	} else {
		return -EINVAL;
	}

	/* update the current policy info */
	ret = cpufreq_frequency_table_cpuinfo(policy, bcm63xx_freq_table);
	if (ret)
		return ret;
	cpufreq_frequency_table_get_attr(bcm63xx_freq_table, policy->cpu);

	/* to get the policy updated with the new freq_table */
	ret = cpufreq_get_policy(&new_policy, policy->cpu);
	if (ret)
		return ret;

	ret = cpufreq_set_policy(policy, &new_policy);
	if (ret)
		return ret;

	return count;
}

static ssize_t show_set_freq_table(struct cpufreq_policy *policy, char *buf)
{
	ssize_t i = 0;

	if (bcm63xx_freq_table == bcm63xx_freq_normal_table)
		i += sprintf(buf, "normal\n");
	else if (bcm63xx_freq_table == bcm63xx_freq_extended_table)
		i += sprintf(buf, "extended\n");
	else
		i += sprintf(buf, "error!\n");

	i += sprintf(&buf[i], "available tables: normal, extended\n");
	return i;
}

cpufreq_freq_attr_rw(set_freq_table);

static int bcm63xx_cpufreq_init_sysfs(struct cpufreq_policy *policy)
{
	/* creating the sysfs for changing freq table */
	int ret = sysfs_create_file(&policy->kobj, &set_freq_table.attr);
	if (ret)
		printk("%s:fail to create sysfs for set_freq_table\n", __func__);

	return ret;
}

static int bcm63xx_cpufreq_cpu_init(struct cpufreq_policy *policy)
{
	int ret;
	policy->cur = policy->min =
		policy->max = bcm63xx_cpufreq_getspeed(policy->cpu);

	/* set the transition latency value */
	policy->cpuinfo.transition_latency = 100000;

	/*
	 * In BCM63xx, all ARM CPUs are set to the same speed.
	 * They all have the same clock source. */
	if (num_online_cpus() == 1) {
		cpumask_copy(policy->related_cpus, cpu_possible_mask);
		cpumask_copy(policy->cpus, cpu_online_mask);
	} else {
		cpumask_setall(policy->cpus);
	}

	ret = cpufreq_frequency_table_cpuinfo(policy, bcm63xx_freq_table);
	if (ret != 0)
		return ret;

	if (policy->cur > policy->max) {
		bcm63xx_freq_table = bcm63xx_freq_extended_table;
		ret = cpufreq_frequency_table_cpuinfo(policy, bcm63xx_freq_table);
		if (ret != 0) {
			/* if unable to set up the extended cpufreq_table, then
			 * we go back use the normal one, it should work */
			bcm63xx_freq_table = bcm63xx_freq_normal_table;
			ret = cpufreq_frequency_table_cpuinfo(policy, bcm63xx_freq_table);
		}
	}

	cpufreq_frequency_table_get_attr(bcm63xx_freq_table, policy->cpu);

	return ret;
}

// TODO! As for October 2013, we do not support PM yet.
#ifdef CONFIG_PM
static int bcm63xx_cpufreq_suspend(struct cpufreq_policy *policy)
{
	return 0;
}

static int bcm63xx_cpufreq_resume(struct cpufreq_policy *policy)
{
	return 0;
}
#endif

static struct freq_attr *bcm63xx_cpufreq_attr[] = {
	&cpufreq_freq_attr_scaling_available_freqs,
	NULL,
};

static struct cpufreq_driver bcm63xx_cpufreq_driver = {
	.flags		= CPUFREQ_STICKY,
	.verify		= bcm63xx_cpufreq_verify_speed,
	.target		= bcm63xx_cpufreq_target,
	.get		= bcm63xx_cpufreq_getspeed,
	.init		= bcm63xx_cpufreq_cpu_init,
	.name		= "bcm63xx_cpufreq",
	.attr		= bcm63xx_cpufreq_attr,
	.init_sysfs	= bcm63xx_cpufreq_init_sysfs,
#ifdef CONFIG_PM
	.suspend	= bcm63xx_cpufreq_suspend,
	.resume		= bcm63xx_cpufreq_resume,
#endif
};

static int __init bcm63xx_cpufreq_init(void)
{
	return cpufreq_register_driver(&bcm63xx_cpufreq_driver);
}
late_initcall(bcm63xx_cpufreq_init);
#endif /* CONFIG_BCM_KF_ARM_BCM963XX */
