# TikZKit

Next to being a palindrome, TikZKit is a graphical what-you-see-is-what-you-get
(WYSIWYG) editor for PGF/TikZ. [PGF/TikZ](http://www.texample.net/tikz/examples/all/)
in turn is a markup package that allows to create most beautiful pictures in LaTeX.
The core features of TikZKit are:
1. Exact rendering as if you had generated the picture through LaTeX.
2. TikZ export, obtaining TikZ code that can easily be manipulated.

Please join tikzkit development to quickly get to a usable state.

## Architecture

TikZKit is written in modern C++ and Qt5.
It is organized in two libraries following the model/view concept, and the application:
1. tikzcore: This library provides the data model. The model entirely defines a
   TikZ picture. The model itself is represented by the class tikz::core::Document.
2. tikzui: This library provides the view. It renders the scene by using the
   QGraphicsView based on a tikz::core::Document.
3. TikZKit: The application is a thin wrapper around the libraries tikzcore and
   tikzui.

## TODO List for a first tikzkit 1.0 release (incomplete)

### tikzcore
- tikz::core::Document (priority: high)
  - makro support: implement Entity::toJsonObject() in all Entity classes. This
    way, Entities can by serialized to JSON, and by replaying the JSON makro,
    items can be e.g. copy & pasted, or template can be created.
  - add tikz::core::Layer: These layers match pgf layers when exported to PGF/TikZ, e.g.
    background, nodes, edges, foreground
  - add tikz::core::Scope: These scopes match \begin{scope} ... \end{scope} in
    PGF/TikZ. These scopes can be used to group items that share a style.

- Add tikz::core::AbstractProperty
  - implement set of required property types (ColorProperty, ValueProperty,
    TextProperty, BoolProperty, StringListProperty, DoubleProperty, PosisionProperty)
  - allow adding / removing properties at runtime, so that the property system
    is extensible e.g. through plugins

- MetaPos: x und y so trennen, dass es von unterschiedlichen Positions/Anchors kommt.
- TexGenerator verbessern (mehrere jobs parallel, saubere kapselung, QTemporaryDir)
- logo
- class Package? -> tikz package + tikz options
- introduce tikz::Editor: add EntityFactoryInterface similar to
  KTE::CommandInterface, add class EntityFactory that returns a list of
  QStrings ("node-rectangle", ... ?
- implement pseudo-plugin that uses the EntityFactory

- tikz::core::Path (priority: high)
  - add support for rectangle primitive
  - add support for grid primitive
  - add support for simple edges (straight, bending, bezier)
  - add support for polygonal lines and shapes

- tikz::core::Node (priority: medium)
  - support anchors above, left, below, right and combinations
  - support anchoring on other anchors

- visitors (priority: high)
  - fix TikZ export Visitor
  - (load visitor, currently unused)
  - (save visitor, currently unused)

### tikzui

- property editor:
  - show properties of the selected item
  - use Transaction to group properties
  - add default/reset button
  - add support for categories and coloring of categories

- LaTeX to SVG generator
  - proper design of the class with cleanup of the metadata (use QTemporaryDir)
  - proper diagnostics, if the dvisvg 

- shapes
  - add more shapes
  - add more arrows

- zoom control: Add widget to display current zoom level

### app
- TikZKit
- add icons to actions

integration test (priority: medium)
- implement testing suite:
  - tests are listed in a list view on the left
  - ability to add a new test
  - test
    - user graphically creates scene: add/manipulate nodes/edges
    - baseline
      - save the scene as TeX/TikZ code and convert to pdf
      - use libpoppler-qt4 to render the pdf from the TeX/TikZ code
      - save pdf as png
      - render TikzScene to png
    - use pdiff to check the graphical diff
    - fail test only on user specified threshold

<!--- kate: space-indent on; indent-width 2; -->
