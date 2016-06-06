#ifndef __testapp_H__
#define __testapp_H__

#include <app.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "testapp"

#if !defined(PACKAGE)
#define PACKAGE "org.example.testapp"
#endif

#endif /* __testapp_H__ */
