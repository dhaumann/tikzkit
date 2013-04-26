#ifndef TIKZ_STYLEMANAGER_H
#define TIKZ_STYLEMANAGER_H

#include <QVector>

namespace tikz {

class Style;

class StyleManager
{
    private:
        static StyleManager* s_self;

    public:
        ~StyleManager();

        static StyleManager* self();

        Style* documentStyle();

        void registerStyle(Style* style);
        void unregisterStyle(Style* style);

    //
    // private constructor, as it is a singleton
    //
    private:
        /**
         * Constructor. Called in ::self() accessor.
         */
        StyleManager();

    private:
        Style* m_documentStyle;
        QVector<Style*> m_styles;
};

}

#endif // TIKZ_STYLEMANAGER_H

// kate: indent-width 4; replace-tabs on;
