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

xgettext -j -C -n -kTranslate -o $po_file $filelist
sed -i 's/charset=CHARSET/charset=UTF-8/g' $po_file