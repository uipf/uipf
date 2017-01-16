#!/bin/sh -e

UIPF=@UIPFramework_BINARY_DIR@/uipf

uipf -l | grep uipf.opencv.imgproc.resize

if ! [ -f improc_resize_1.yml ] ; then
    exit 1
fi
rm -f test_resize.jpg
$UIPF -c improc_resize_1.yml > chainout.txt || (cat chainout.txt ; exit 1)

if ! [ -f test_resize.jpg ] ; then
    cat chainout.txt
    echo "result image file does not exist."
    exit 1
fi

# TODO verify image size