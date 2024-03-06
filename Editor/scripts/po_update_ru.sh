filelist=`find ../src -type f \( -iname \*.cpp -o -iname \*.h \)`
echo ${filelist}

po_file=../locale/ru_RU/LC_MESSAGES/StorytellerEditor.po
if [ -f "$po_file" ]; then
	echo "updating $po_file"
else
	echo "initializing $po_file"
	po_dir=`dirname $po_file`
	[ ! -d $po_dir ] && mkdir -p $po_dir
	touch $po_file
fi

xgettext -j -C -n -kTranslate:2,2t -kTranslate:2,3,4t -kTranslateCtx:2,3c,3t -kTranslateCtx:2,3,5t,5t -o $po_file $filelist
sed -i 's|charset=CHARSET|charset=UTF-8|g' $po_file
sed -i 's|nplurals=INTEGER|nplurals=3|g' $po_file
sed -i 's|plural=EXPRESSION|plural=n%10==1 \&\& n%100!=11 ? 0 : n%10>=2 \&\& n%10<=4 \&\& (n%100<10 \|\| n%100>=20) ? 1 : 2|g' $po_file