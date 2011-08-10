#! /bin/sh

vfi=`echo $1 | sed 's/\.tex//g'`

echo "\\documentclass{article}"
echo "\\usepackage{pst-all}"
echo "\\\\begin{document}"
echo "\\\\begin{figure}"
echo "\\\\begin{center}"

echo "\\\\include{$vfi}"

echo "\\\\caption{Here is a picture}"
echo "\\\\label{SOMEFIGURE}"
echo "\\\\end{center}"
echo "\\\\end{figure}"
echo "Here is a ref to Figure \\\\ref{SOMEFIGURE} huzzah."
echo "\\end{document}"
