#!/bin/bash

VICTOR_EMAIL="dodonvictor at gmail dot com"
VOVA_EMAIL="kaspersky_gg@gmail.com"
VITALIK_EMAIL="maldur.vitalik@yahoo.com"
VICU_EMAIL="straticiuc_vicu@yahoo.com"

for file in *.h *.cpp; do
	sed -ie '1,13s/Dodon Victor <.*>/Dodon Victor <'"$VICTOR_EMAIL"'>/' $file;
	sed -ie '1,13s/Cernov Vladimir <.*>/Cernov Vladimir <'"$VOVA_EMAIL"'>/' $file;
	sed -ie '1,13s/Straticiuc Vicu <.*>/Straticiuc Vicu <'"$VICU_EMAIL"'>/' $file;
	sed -ie '1,13s/Maldur Vitalie <.*>/Maldur Vitalie <'"$VITALIK_EMAIL"'>/' $file;
done

rm *.he *.cppe
