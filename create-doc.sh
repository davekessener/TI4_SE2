#!/bin/bash

git_root=`git rev-parse --show-toplevel`
doc_path="doc/doxygen"

cd "$git_root"
doxygen "$doc_path/doxy.config"
make -C "$doc_path/latex" pdf
cp "$doc_path/latex/refman.pdf" "$doc_path/doc.pdf"

