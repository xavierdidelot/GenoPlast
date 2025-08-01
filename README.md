# GenoPlast

## Introduction

Gain and loss of genetic material are essential forces in
bacterial microevolution. They have been repeatedly linked with
adaptation of lineages to new lifestyles, and in particular
pathogenicity. GenoPlast implements a model-based method for using whole
genome sequences to infer the patterns of genome content evolution. The
method is described in
<a href="http://genome.cshlp.org/content/early/2008/11/17/gr.082263.108.abstract">Didelot,
Darling and Falush (2009)</a>. A fundamental property of our model is
that it allows the rates at which genetic elements are gained or lost to
vary in time and from one lineage to another.

## Important notice

GenoPlast is no longer actively maintained.

## Installation

Installation is done either via the qmake utility.
First use the command `qmake` to generate the `Makefile`.
Then use the command `make` to compile.

## Usage

Command line usage is as follows: 
  
`genoplast [OPTIONS] treefile datafile outputfile`

The options are:
  
-x NUM      Sets the number of burn-in iterations (default is 10000)  
-y NUM      Sets the number of iterations after burn-in (default is 10000)  
-z NUM      Sets the number of iterations between samples (default is 10)
-s NUM      Use given seed to initial random number generator  
-A          Output ancestral states  
-v          Verbose mode

## Example

The directory `example` contains examples of the tree file (in Newick format), data file (in tabular format) and output file (in XML format).
