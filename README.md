# mc_euclid

Building blueprint generator for euclidean objects defined by polynomial equations.

The blueprints are to be used in voxel-based games (e.g. Minecraft).

## Usage

>$ ./mc_euclid -s [solid] -o [dir] -f [scale]

>		solid:	 The euclidean solid to be generated.

>			Accepted solids are:

>				cube.json		A cube (diagonally posed)

>				sphere.json		A sphere

>				torus.json		A torus with a 1/3 whole

>				torus2.json		A torus with a 1/2 whole

>				tetrahedron.json	A tetrahedron

>				rightcube.json		An upright cube (PoC)

>		dir:	 The directory where the maps will go

>		scale:	 The scale factor of the solid

## Output

The output consists of a folder with the desired plans within, in *.xpm* format.

Each layer has its own blueprint, and therefore its own image.
