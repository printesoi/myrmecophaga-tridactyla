#!/bin/bash

VOVA_EMAIL="gg.kaspersky@gmail.com"
VICTOR_EMAIL="dodonvictor at gmail dot com"
VITALIK_EMAIL="maldur.vitalik@yahoo.com"
VICU_EMAIL="straticiuc_vicu@yahoo.com"

for file in *.h *.cpp; do
	sed -ie '1,13s/Vladimir Cernov <.*>/Vladimir Cernov <'"$VOVA_EMAIL"'>/' $file;
	sed -ie '1,13s/Victor Dodon <.*>/Victor Dodon <'"$VICTOR_EMAIL"'>/' $file;
	sed -ie '1,13s/Vicu Straticiuc <.*>/Vicu Straticiuc <'"$VICU_EMAIL"'>/' $file;
	sed -ie '1,13s/Vitalie Maldur <.*>/Vitalie Maldur <'"$VITALIK_EMAIL"'>/' $file;
done

rm *.he *.cppe
