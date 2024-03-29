
## geojsonR
<br>

The **geojsonR** package includes functions for processing [GeoJson objects](https://en.wikipedia.org/wiki/GeoJSON) relying on [RFC 7946](https://datatracker.ietf.org/doc/pdf/rfc7946.pdf). The geojson encoding is based on [json11](https://github.com/dropbox/json11), a tiny JSON library for C++11. Furthermore, the source code is exported in R through the *Rcpp* and *RcppArmadillo* packages. More details on the functionality of geojsonR can be found in the [blog-post](http://mlampros.github.io/2017/03/29/geojsonR_package/) and in the package Vignette.
<br><br>

To install the package from CRAN use, 

```R

install.packages("geojsonR")


```
<br>

and to download the latest version from Github use the *install_github* function of the *remotes* package,
<br><br>

```R

remotes::install_github('mlampros/geojsonR')


```
<br>

Use the following link to report bugs/issues,
<br><br>

[https://github.com/mlampros/geojsonR/issues](https://github.com/mlampros/geojsonR/issues)

<br>

### **Citation:**

If you use the code of this repository in your paper or research please cite both **geojsonR** and the **original articles / software** `https://CRAN.R-project.org/package=geojsonR`:

<br>

```R
@Manual{,
  title = {{geojsonR}: A GeoJson Processing Toolkit},
  author = {Lampros Mouselimis},
  year = {2021},
  note = {R package version 1.1.1},
  url = {https://CRAN.R-project.org/package=geojsonR},
}
```

<br>
