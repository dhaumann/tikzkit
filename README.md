# TikZKit

Next to being a palindrome, TikZKit is a graphical what-you-see-is-what-you-get (WYSIWYG) editor for PGF/TikZ.
[PGF/TikZ](http://www.texample.net/tikz/examples/all/) in turn is a markup
package that allows to create most beautiful pictures in LaTeX.
The core features of TikZKit are:
1. Exact rendering as if you had generated the picture through LaTeX.
2. TikZ export, obtaining TikZ code that can easily be manipulated.

## Architecture

TikZKit is written in C++11 and Qt5.
It is organized in two libraries following the model/view concept, and the application:
1. tikzcore: This library describes the model. A tikz picture is entirely described by a tikz::core::Document.
2. tikzui: This library describes the view. It renders the scene by using the QGraphicsView based on a tikz::core::Document.
3. TikZKit: The application is a wrapper around the libraries tikzcore and tikzui.

## TODO List (incomplete)

### tikzcore
- tikz::core::Document (priority: high)
  - maybe use pgf layers: background, nodes, edges, foreground

- visitors (priority: high)
  - fix TikZ export Visitor
  - fix load visitor
  - fix save visitor

- tikz::Path (priority: high)
  - add support for rectangle primitive
  - add support for grid primitive
  - add support for simple edges (straight, bending, bezier)
  - add support for polygonal lines and shapes

- tikz::Node (priority: medium)
  - support anchors above, left, below, right and combinations
  - support anchoring on other anchors

### tikzui

- shapes
  - add more shapes
  - add more arrows

### app
- TikZKit

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