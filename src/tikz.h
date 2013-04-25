#ifndef TIKZ_H
#define TIKZ_H

namespace tikz
{

enum Anchor {
    AnchorUnset = 0,
    Center,
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest
};

enum Shape {
    ShapeUnset = 0,
    NoShape,
    ShapeCircle,
    ShapeRectangle
};

enum PenStyle {
    PenUnset = 0,
    NoPen,
    SolidLine,
    DashLine,
    DashDotLine
};

enum LineWidth {
    WidthUnset = 0,
    UltraThin,
    VeryThin,
    Thin,
    Normal,
    Thick,
    VeryThick,
    UltraThick
};

enum ArrowStyle {
    ArrowUnset = 0,
    NoArrow,
    ArrowStealth
};

}

#endif // TIKZ_H

// kate: indent-width 4; replace-tabs on;
