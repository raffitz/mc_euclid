# mc_euclid

Building blueprint generator for euclidean objects defined by polynomial equations.

The blueprints are to be used in voxel-based games (e.g. Minecraft).

## Build

> cd src && make

## Options

 - [x] `-a`, `--ascii`: ascii output to cli (default)
 - [ ] `-u`, `--unicode`: unicode output to cli (default)
 - [ ] `-w`, `--wide`: wide ascii output to cli (default)
 - [ ] `-x`, `--xpm`: xpm output
 - [ ] `-s`, `--svg`: svg output
 - [ ] `-g`, `--grid`: grid
 - [ ] `-l`, `--label`: label

## Output

The output consists of a series of images, each depicting a layer of the solid.

## Current Shapes

#### Sphere

A sphere

#### Torus

A torus where the radius of the tube is 1/4th the radius of the entire torus

#### Torus2

A torus where the radius of the tube is 1/3rd the radius of the entire torus

#### Cube

A cube stood on one of its vertices

#### Tetrahedron

A tetrahedron stood on one of its faces (for it to be stood on its top vertex, you need only read the maps in reverse order)

#### Octahedron

An octahedron stood on one of its vertices (rather pointless as well)

#### Dodecahedron

A dodecahedron stood on one of its faces

#### Icosahedron

An icosahedron stood on one of its faces

#### Dodecahedron (dual)

A dodecahedron stood on one of its vertices, made from the duality of the icosahedron

#### Icosahedron (dual)

An icosahedron stood on one of its vertices, made from the duality of the dodecahedron

#### Pentagon

A 2D shape, a regular pentagon
