#ifndef __SYS_TASK_H__
#define __SYS_TASK_H__

void SysTaskInit(void);

#if ((SOFTWARE_MODE == SOFTWARE_MODE_SERIAL) || (SOFTWARE_MODE == SOFTWARE_MODE_ANOTECH))
	void vTaskGetBugs(void *args);
#elif SOFTWARE_MODE == SOFTWARE_MODE_RELEASE
	
#endif

#endif /* __SYS_TASK_H__ */
