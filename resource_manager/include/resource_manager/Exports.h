
#ifndef RESOURCE_MANAGER_EXPORT_H
#define RESOURCE_MANAGER_EXPORT_H

#ifdef RESOURCE_MANAGER_STATIC_DEFINE
#  define RESOURCE_MANAGER_EXPORT
#  define RESOURCE_MANAGER_NO_EXPORT
#else
#  ifndef RESOURCE_MANAGER_EXPORT
#    ifdef resource_manager_EXPORTS
        /* We are building this library */
#      define RESOURCE_MANAGER_EXPORT 
#    else
        /* We are using this library */
#      define RESOURCE_MANAGER_EXPORT 
#    endif
#  endif

#  ifndef RESOURCE_MANAGER_NO_EXPORT
#    define RESOURCE_MANAGER_NO_EXPORT 
#  endif
#endif

#ifndef RESOURCE_MANAGER_DEPRECATED
#  define RESOURCE_MANAGER_DEPRECATED __declspec(deprecated)
#endif

#ifndef RESOURCE_MANAGER_DEPRECATED_EXPORT
#  define RESOURCE_MANAGER_DEPRECATED_EXPORT RESOURCE_MANAGER_EXPORT RESOURCE_MANAGER_DEPRECATED
#endif

#ifndef RESOURCE_MANAGER_DEPRECATED_NO_EXPORT
#  define RESOURCE_MANAGER_DEPRECATED_NO_EXPORT RESOURCE_MANAGER_NO_EXPORT RESOURCE_MANAGER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef RESOURCE_MANAGER_NO_DEPRECATED
#    define RESOURCE_MANAGER_NO_DEPRECATED
#  endif
#endif

#endif /* RESOURCE_MANAGER_EXPORT_H */
