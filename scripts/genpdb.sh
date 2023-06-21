#!/bin/bash
# mkdir-wrapper to create directories only when required
set -e

PDB="$OUT/pdb"

eval $TOPDIR/scripts/mkdir.sh "$PDB"

if [ -z "$1" ]; then
    echo "genpdb: invalid arguments"
fi

cv2pdb "$OUT/$1" "${PDB}/${1}" "${OUT}/${1%.*}.pdb" 2>>"$OUT/genpdb.log" || true

echo "genpdb: done"

exit 0