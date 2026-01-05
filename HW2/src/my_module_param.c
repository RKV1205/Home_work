#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/stat.h>
#include <linux/string.h>

#define MAX_SIZE 13
#define LAST_ELEMENT (MAX_SIZE-1)

static int idx = 0;//индекс элемента в строке

static char ch_val = ' ';//записываемый символ в строку
static char str_buf[MAX_SIZE] ;//изменяемая строка

static void SetCharToString(void);
/////////////////////////////////////////////////////////////////////////////////////////////--------idx
//функции для доступа из структур kernel_param_ops
static int idx_param_get(char *buf, const struct kernel_param *kp)
{
    return sprintf(buf, "%d", idx);
}

static int idx_param_set(const char *val, const struct kernel_param *kp)
{
    int ret;
	int tempVal = 0;
    ret = kstrtoint(val, 10, &tempVal);
    if(ret) {
        return ret; }
	if(tempVal>=0 && tempVal < MAX_SIZE){
	idx = tempVal;
    pr_info("Индекс idx value = %d\n", idx);
		SetCharToString();	
	}
	else 
	{
		idx = 0;
	pr_err("Введёный индекс %d вне диапазаона строки (0-12 символов)\n",tempVal);
	return -EINVAL	;
	}
	
    return 0;
}

//доступные операции с параметрами
static const struct kernel_param_ops idx_params = {
    .set = idx_param_set,
    .get = idx_param_get,
};
//макросы для доступа к параметрам idx
module_param_cb(idx, &idx_params, &idx, 0664);
MODULE_PARM_DESC(idx, "Индекс символа в строке(диапазон: 0-12)");
/////////////////////////////////////////////////////////////////////////////////////////////--------ch_val
static int ch_val_param_get(char *buf, const struct kernel_param *kp)
{
    return sprintf(buf, "%c", ch_val);
}

static int ch_val_param_set(const char *val, const struct kernel_param *kp)
{
	if(*val>=' '&& *val < '~' ){
    pr_info("Введён символ = %c в позицию %d\n", *val, idx);
	ch_val = *val;
	SetCharToString();
	}
	else 
	{
	ch_val = ' ';
	pr_err("Введёный символ не используемого диапазона(' '-'~')");
	return -EINVAL	;
	}
	
    return 0;
}

static void SetCharToString(void)
{
int i ;
	for(i =0 ; i < MAX_SIZE;i++)
	{
		if(i<idx && str_buf[i]=='\0') str_buf[i]=' ';
		if(i==idx)str_buf[i]=ch_val;
	}
}

//доступные операции с параметрами
static const struct kernel_param_ops ch_val_params = {
    .set = ch_val_param_set,
    .get = ch_val_param_get,
};
//макросы для доступа к параметрам ch_val
module_param_cb(ch_val, &ch_val_params, &ch_val, 0664);
MODULE_PARM_DESC(ch_val, "Записываемый символ в строку(' '-'~')");

/////////////////////////////////////////////////////////////////////////////////////////////--------string
static int str_param_get(char *buf, const struct kernel_param *kp)
{
    return sprintf(buf, "%s", str_buf);
}

//доступные операции с параметрами
static const struct kernel_param_ops str_params = {
   // .set = str_param_set,
    .get = str_param_get,
};


//макросы для доступа к параметрам str_buf
module_param_cb(str_buf, &str_params, &str_buf, 0444);
MODULE_PARM_DESC(str_buf, "Строка для чтения");




static int __init mod_init(void) {
  pr_info("init \n");

  return 0;
}

static void __exit mod_exit(void) { 
	pr_info("exit\n"); 
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Рогожин Кирилл Владимирович");
MODULE_DESCRIPTION("Домашнее задание #2");