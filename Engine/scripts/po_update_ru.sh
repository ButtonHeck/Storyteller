filelist=`find ../src ../include -type f \( -iname \*.cpp -o -iname \*.h \)`
echo ${filelist} | tr ' ' '\n'

pot_file=../locale/ru_RU/LC_MESSAGES/Storyteller.pot
if [ -f "$pot_file" ]; then
	echo "updating $pot_file"
else
	echo "initializing $pot_file"
	po_dir=`dirname $pot_file`
	[ ! -d $po_dir ] && mkdir -p $po_dir
	touch $pot_file
fi

xgettext -j -C -n -kTranslate:2,2t -kTranslate:2,3,4t -kTranslateCtx:2,3c,3t -kTranslateCtx:2,3,5t,5t -kTranslateDefer:2,2t -kTranslateDefer:2,3,4t -kTranslateCtxDefer:2,3c,3t -kTranslateCtxDefer:2,3,5t,5t -o $pot_file $filelist
sed -i 's|Language:|Language: ru|g' $pot_file
sed -i 's|charset=CHARSET|charset=UTF-8|g' $pot_file
sed -i 's|nplurals=INTEGER|nplurals=3|g' $pot_file
sed -i 's|plural=EXPRESSION|plural=n%10==1 \&\& n%100!=11 ? 0 : n%10>=2 \&\& n%10<=4 \&\& (n%100<10 \|\| n%100>=20) ? 1 : 2|g' $pot_file


po_file=../locale/ru_RU/LC_MESSAGES/Storyteller.po
if [ ! -f "$po_file" ]; then
	echo "initializing $po_file"
	msginit -i $pot_file -o $po_file -l ru_RU
fi

msgattrib --set-obsolete --ignore-file=$pot_file -o $po_file $po_file
msgattrib --no-obsolete -o $po_file $po_file
msgmerge -q -o $po_file $po_file $pot_file

rm $pot_file