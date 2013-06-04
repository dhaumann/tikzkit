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
    NoShape = 0,
    ShapeRectangle,
    ShapeCircle,
    ShapeEllipse,
};

enum PenStyle {
    NoPen = 0,
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
    UltraThin = 0,
    VeryThin,
    Thin,
    SemiThick,  // TikZ default
    Thick,
    VeryThick,
    UltraThick,
    CustomLineWidth
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

enum Arrow {
    ArrowUnset = 0,
    NoArrow,
    ToArrow,
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
