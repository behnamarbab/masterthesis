for i in *.svg; do 
    inkscape "$i" -e ${i/.svg}.png
done
