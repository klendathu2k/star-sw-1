# StarRoot

## Purpose

StarRoot provides utility classes used throughout the STAR reconstruction and analysis
framework.  It supplies track and vertex data structures, helix propagation tools,
matrix algebra primitives, numerical methods, and ROOT/framework utilities that are
shared across many STAR packages.

## Dependencies

StarRoot depends on **ROOT** (TObject, TArrayD, TF1, TTree, etc.) and
**StarClassLibrary** (StThreeVectorD).

---

## Key Class Groups

### Track & Vertex

| Class | Description |
|-------|-------------|
| `MTrack` | Lightweight track: 6 state parameters (x,y,z,px,py,pz), 21-element covariance, chi2/NDF/charge. |
| `MVertex` | Lightweight vertex: (x,y,z) position, 6-element covariance, chi2/NDF, contributor count. |
| `VVertex` | Abstract TNamed base for ESD/AOD vertices; defines the pure-virtual position/covariance interface. |
| `TCFitV0` | V0 decay constrained fitter (two tracks + energy-conservation constraint) built on TCFit. |

### Helix & Tracking

| Class | Description |
|-------|-------------|
| `THelixTrack` | Full 3D helix with 5-parameter error matrix (THEmx_t); arc-length stepping and DCA. |
| `THelix3d` | 3D helix with magnetic-field-aware propagation and THEmx3d_t error transport. |
| `TRungeKutta` | Fourth-order Runge–Kutta propagator in a non-uniform field (TRKuttaMag functor). |
| `TCircleFitter` | Least-squares 2D circle fitter accumulating (x,y,z) hits. |
| `THelixFitter` | Combined circle + z-polynomial helix fitter wrapping TCircleFitter. |

### Matrix Algebra

| Class | Description |
|-------|-------------|
| `TRArray` | Base double-precision array with matrix-type enum and arithmetic operators. |
| `TRMatrix` | Rectangular matrix; row-major storage; element (i,j) → j + i·ncols. |
| `TRSymMatrix` | Symmetric matrix; packed lower-triangular storage; in-place inversion via TCL::trsinv. |
| `TRDiagMatrix` | Diagonal matrix storing only the n diagonal elements. |
| `TRVector` | Column vector inheriting TRMatrix; adds Cross(), Unit(), and 3D conversions. |

### Numerical Methods

| Class | Description |
|-------|-------------|
| `TPolinom` | Polynomial with coefficient covariance for Gaussian error propagation. |
| `TPoliFitter` | Least-squares polynomial fitter using orthogonal basis polynomials. |
| `TNumDeriv` | Abstract base for numerical differentiation; override Fcn() to define the function. |
| `TRandomVector` | Generates correlated Gaussian random vectors from a symmetric covariance matrix. |
| `TMDFParameters` | Multi-dimensional Chebyshev polynomial evaluator with per-coefficient uncertainties. |

### ROOT Utilities

| Class | Description |
|-------|-------------|
| `TAttr` | Named attribute map: store/retrieve string, integer, and double values by key. |
| `TUnixTime` | UNIX timestamp with local/GMT conversion and ROOT TDatime interoperability. |
| `TTreeIter` | TTree/TChain iterator with type-safe branch access by name. |
| `TH1Helper` | Histogram statistics helper: mean, RMS, and integral over a sub-range. |
| `TF1F` | TF1 with 200-point default sampling and interpolated save-array lookup. |

### System Utilities

| Class | Description |
|-------|-------------|
| `TDirIter` | Recursive directory iterator with optional regex file filtering. |
| `StCloseFileOnTerminate` | Singleton signal handler that closes all ROOT files on SIGTERM. |
| `StCheckQtEnv` | Static helpers to configure Qt/ROOT plugin entries in the ROOT resource file. |
| `THack` | Static utilities for low-level ROOT object manipulation (TClonesArray, pads, etc.). |
| `xTCL` | Static linear-algebra utilities: matrix products, eigenvalues, Simpson integration, QP. |
