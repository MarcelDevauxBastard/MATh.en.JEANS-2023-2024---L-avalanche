# MATh.en.JEANS 2023-2024 - L'avalanche

This little project had to solve a problem proposed by MATh.en.JEANS 2023-2024 via computer science.

## The problem: L'avalanche

Un skieur est pris dans une avalanche, heureusement il est muni d'un ARVA. Un sauveteur disposant également d'un ARVA arrive en un point du bord de l'avalanche (qu'il ne choisit pas).
Il utilise la stratégie suivante : il suit une ligne droite, trouve l'endroit où le signal est maximal puis choisit la direction perpendiculaire...etc.

Quelle est la meilleure direction initiale que doit choisir le sauveteur lorsqu'il arrive sur l'avalanche ?

On pourra considérer dans un premier temps que l'avalanche a une forme triangulaire.
On considérera que l'ARVA émet un signal identique dans toutes les directions.
Il faudra sans doute décider quel est le sens que l'on veut donner à la meilleure direction à prendre.


A skier is caught in an avalanche, fortunately he is equipped with an ARVA. A rescuer who also has an ARVA arrives at a point on the edge of the avalanche (which he does not choose).
It uses the following strategy: it follows a straight line, finds the place where the signal is maximum then chooses the perpendicular direction...etc.

What is the best initial direction for the rescuer to choose when arriving at the avalanche?

We can initially consider that the avalanche has a triangular shape.
We will consider that the ARVA emits an identical signal in all directions.
We will undoubtedly have to decide what meaning we want to give to the best direction to take.

## How does it work ?

* The program takes the vertices of the avalanche as input.
* It considers that the rescuer starts its searchings at the first vertex.
* (If you want it to start on an edge, just draw a flat angle.)
* The best angle is the one that minimizes the travel distance between the rescuer and each point. This angle is between 0 and 2pi.
* The program defines a line and 1 000 000 random points in the figure.
* It then computes the distances between the rescuer and each point present on either sides of the line.
* The travel distance is minimal when the average distance to the points on both sides are equal.
* We find the best angle using a dichotomy process (we adjust the angle of the line by a two times smaller correction at each iteration).
* That way, we approach the result very quickly.

## How to use it ?

### Installation

* There will be no release as this project is experimental.
* If you want to use it or some parts of it in you code, just clone the repository.

### Usage

* Launch the program.
* Press `left click` to place a vertex.
* Press `backspace` to delete the last one.
* Press `equal` to delete the whole figure/reset the program.
* Press `right click` to toggle fixed point mode (rounds the coordinates to draw prettier figures).
* Use the `mousewheel` to zoom/dezoom on the cursor.
* Use `W/A/S/D` to move around.
* Press `escape` to reset the camera.
* Press `enter` a few times to get the best angle.
* Close the window to end the program.

## Built with

* Compiled using [MinGW-w64](https://www.mingw-w64.org/)
* Built with [SDL](https://www.libsdl.org/)
