# BPSO

This is a parallel and distributed mono-objective algorithm to EEG classification. It corresponds to a Binary Particle Swarm Optimization. The application follows a master-worker model, being able to splitthe evaluation of all the particles among all available nodes. Inside each of these nodes, it´s able to evaluate the particles using all the CPUs, providing up to 2 leves of parallelism. 

## Requirements

This BPSO requires a GCC compiler. It also depends on the following APIs and libraries:

* [OpenMPI](https://www.open-mpi.org/doc/current/).


## Funding

This work has been funded by:

* Spanish [*Ministerio de Ciencia, Innovación y Universidades*](https://www.ciencia.gob.es/) under grant number PGC2018-098813-B-C31.
* [*European Regional Development Fund (ERDF)*](https://ec.europa.eu/regional_policy/en/funding/erdf/).

<div style="text-align: right">
  <img src="https://raw.githubusercontent.com/efficomp/Hpmoon/main/docs/logos/miciu.jpg" height="60">
  <img src="https://raw.githubusercontent.com/efficomp/Hpmoon/main/docs/logos/erdf.png" height="60">
</div>

