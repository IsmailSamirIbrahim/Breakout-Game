
#ifndef TEXTURE_EXPORT_H
#define TEXTURE_EXPORT_H

#ifdef TEXTURE_STATIC_DEFINE
#  define TEXTURE_EXPORT
#  define TEXTURE_NO_EXPORT
#else
#  ifndef TEXTURE_EXPORT
#    ifdef texture_EXPORTS
        /* We are building this library */
#      define TEXTURE_EXPORT 
#    else
        /* We are using this library */
#      define TEXTURE_EXPORT 
#    endif
#  endif

#  ifndef TEXTURE_NO_EXPORT
#    define TEXTURE_NO_EXPORT 
#  endif
#endif

#ifndef TEXTURE_DEPRECATED
#  define TEXTURE_DEPRECATED __declspec(deprecated)
#endif

#ifndef TEXTURE_DEPRECATED_EXPORT
#  define TEXTURE_DEPRECATED_EXPORT TEXTURE_EXPORT TEXTURE_DEPRECATED
#endif

#ifndef TEXTURE_DEPRECATED_NO_EXPORT
#  define TEXTURE_DEPRECATED_NO_EXPORT TEXTURE_NO_EXPORT TEXTURE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef TEXTURE_NO_DEPRECATED
#    define TEXTURE_NO_DEPRECATED
#  endif
#endif

#endif /* TEXTURE_EXPORT_H */
