#ifndef CONFIG_PARS
#define CONFIG_PARS
typedef struct {
    int pwd_buff;
    int prompt_buff;
} config;

void search_config(config *cfg);
#endif