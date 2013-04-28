#ifndef TIKZ_EXPORT_H
#define TIKZ_EXPORT_H

// visibility
#define HAVE_VISIBILITY

#ifdef HAVE_VISIBILITY
# define PROPERTY_NO_EXPORT __attribute__ ((visibility("hidden")))
# define PROPERTY_EXPORT __attribute__ ((visibility("default")))
# define PROPERTY_IMPORT __attribute__ ((visibility("default")))
#else
# define PROPERTY_NO_EXPORT
# define PROPERTY_EXPORT
# define PROPERTY_IMPORT
#endif

#ifndef TIKZCORE_EXPORT
# if defined(MAKE_TIKZCORE)
/* We are building this library */
#  define TIKZCORE_EXPORT PROPERTY_EXPORT
# else
/* We are using this library */
#  define TIKZCORE_EXPORT PROPERTY_IMPORT
# endif
#endif

#endif // TIKZ_EXPORT_H

// kate: indent-width 4; replace-tabs on;
