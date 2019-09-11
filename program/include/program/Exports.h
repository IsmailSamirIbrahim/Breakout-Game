
#ifndef PROGRAM_EXPORT_H
#define PROGRAM_EXPORT_H

#ifdef PROGRAM_STATIC_DEFINE
#  define PROGRAM_EXPORT
#  define PROGRAM_NO_EXPORT
#else
#  ifndef PROGRAM_EXPORT
#    ifdef program_EXPORTS
        /* We are building this library */
#      define PROGRAM_EXPORT 
#    else
        /* We are using this library */
#      define PROGRAM_EXPORT 
#    endif
#  endif

#  ifndef PROGRAM_NO_EXPORT
#    define PROGRAM_NO_EXPORT 
#  endif
#endif

#ifndef PROGRAM_DEPRECATED
#  define PROGRAM_DEPRECATED __declspec(deprecated)
#endif

#ifndef PROGRAM_DEPRECATED_EXPORT
#  define PROGRAM_DEPRECATED_EXPORT PROGRAM_EXPORT PROGRAM_DEPRECATED
#endif

#ifndef PROGRAM_DEPRECATED_NO_EXPORT
#  define PROGRAM_DEPRECATED_NO_EXPORT PROGRAM_NO_EXPORT PROGRAM_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PROGRAM_NO_DEPRECATED
#    define PROGRAM_NO_DEPRECATED
#  endif
#endif

#endif /* PROGRAM_EXPORT_H */
