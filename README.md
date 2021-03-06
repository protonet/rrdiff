# rrdiff - Ruby bindings to librsync/rdiff.

Lets say you have two files oldfile and newfile where newfile
is a modified version of oldfile. We want to be able to apply
the change to oldfile such that it becomes newfile.

The first process is to create a signature for the oldfile. The
parameters are the original file and where you want to store
the signature file.

    RRDiff.signature("oldfile", "sigfile")

Then from the newfile and the sigfile generate a delta. Deltafile
is where the delta is stored.

    RRDiff.delta("newfile", "sigfile", "deltafile")

Finally, apply the deltafile to the oldfile to create the patchedfile
which should be equivalent to newfile.

    RRDiff.patch("oldfile", "deltafile", "patchedfile")

There is also a File interface, that you can pass File instances. 
It will return Tempfiles.

    RRDiff::File.signature(oldfile) #=> <File>

    RRDiff::File.delta(sigfile, newfile) #=> <File>

    RRDiff::File.diff(oldfile, newfile) #=> <File>

    RRDiff::File.patch(oldfile, deltafile) #=> <File>
