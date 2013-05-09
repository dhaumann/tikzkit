#ifndef TIKZ_H
#define TIKZ_H

namespace tikz
{

enum Anchor {
    NoAnchor = 0,
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
    DottedLine,
    DenselyDottedLine,
    LooselyDottedLine,
    DashedLine,
    DenselyDashedLine,
    LooselyDashedLine,
    DashDottedLine,
    DenselyDashDottedLine,
    LooselyDashDottedLine,
    DashDotDottedLine,
    DenselyDashDotDottedLine,
    LooselyDashDotDottedLine
};

enum LineWidth {
    WidthUnset = 0,
    UltraThin,
    VeryThin,
    Thin,
    SemiThick,  // TikZ default
    Thick,
    VeryThick,
    UltraThick
};

enum LineCap {
    CapUnset = 0,
    RoundCap,
    RectCap,
    ButtCap     // TikZ default
};

enum LineJoin {
    JoinUnset = 0,
    RoundJoin,
    BevelJoin,
    MiterJoin    // TikZ default
};

enum ArrowStyle {
    ArrowUnset = 0,
    NoArrow,
    ArrowStealth
};

enum CurveMode {
    BendCurve = 0,
    BezierCurve,
    InOutCurve
};

}

#endif // TIKZ_H

// kate: indent-width 4; replace-tabs on;
