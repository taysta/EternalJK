# user-specific values
# specify the actual values in a separate file called signing_values.local

# ****************************************************************************************
# identity as specified in Keychain
SIGNING_IDENTITY="Developer ID Application: Your Name (XXXXXXXXX)"

ASC_USERNAME="your@apple.id"

# signing password is app-specific (https://appleid.apple.com/account/manage) and stored in Keychain (as "notarize-app" in this case)
ASC_PASSWORD="@keychain:notarize-app"

# ProviderShortname can be found with
# xcrun altool --list-providers -u your@apple.id -p "@keychain:notarize-app"
ASC_PROVIDER="XXXXXXXXX"
# ****************************************************************************************

source ../taysta/signing_values.local

# Pre-notarized zip file (not what is shipped)
PRE_NOTARIZED_ZIP="${PRODUCT_NAME}_prenotarized.zip"

# Post-notarized zip file (shipped)
POST_NOTARIZED_ZIP="${PRODUCT_NAME}_notarized_$(date +"%Y-%m-%d").zip"

# sign the resulting app bundle
echo "signing..."
if [ "$2" == "entitlements" ]; then
	echo codesign --force --options runtime --deep  --entitlements "${ENTITLEMENTS_FILE}" --sign "${SIGNING_IDENTITY}" "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}"
	codesign --force --options runtime --deep  --entitlements "${ENTITLEMENTS_FILE}" --sign "${SIGNING_IDENTITY}" "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}"
else
	echo codesign --force --options runtime --deep --sign "${SIGNING_IDENTITY}" "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}"
	codesign --force --options runtime --deep --sign "${SIGNING_IDENTITY}" "${BUILT_PRODUCTS_DIR}/${WRAPPER_NAME}"
fi

if [ "$1" == "notarize" ]; then

    cd ${BUILT_PRODUCTS_DIR}

	# notarize app
	# script taken from https://github.com/rednoah/notarize-app

	# create the zip to send to the notarization service
	echo "zipping..."
	ditto -c -k --sequesterRsrc --keepParent "${WRAPPER_NAME}" "${PRE_NOTARIZED_ZIP}"

	# create temporary files
	NOTARIZE_APP_LOG=$(mktemp -t notarize-app)
	NOTARIZE_INFO_LOG=$(mktemp -t notarize-info)

	# delete temporary files on exit
	function finish {
		rm "$NOTARIZE_APP_LOG" "$NOTARIZE_INFO_LOG"
	}
	trap finish EXIT

	echo "submitting..."
	# submit app for notarization
	if xcrun altool --notarize-app --primary-bundle-id "$BUNDLE_ID" --asc-provider "$ASC_PROVIDER" --username "$ASC_USERNAME" --password "$ASC_PASSWORD" -f "$PRE_NOTARIZED_ZIP" > "$NOTARIZE_APP_LOG" 2>&1; then
		cat "$NOTARIZE_APP_LOG"
		RequestUUID=$(awk -F ' = ' '/RequestUUID/ {print $2}' "$NOTARIZE_APP_LOG")

		# check status periodically
		while sleep 60 && date; do
			# check notarization status
			if xcrun altool --notarization-info "$RequestUUID" --asc-provider "$ASC_PROVIDER" --username "$ASC_USERNAME" --password "$ASC_PASSWORD" > "$NOTARIZE_INFO_LOG" 2>&1; then
				cat "$NOTARIZE_INFO_LOG"

				# once notarization is complete, run stapler and exit
				if ! grep -q "Status: in progress" "$NOTARIZE_INFO_LOG"; then
					xcrun stapler staple "$WRAPPER_NAME"
					break
				fi
			else
				cat "$NOTARIZE_INFO_LOG" 1>&2
				exit 1
			fi
		done
	else
		cat "$NOTARIZE_APP_LOG" 1>&2
		exit 1
	fi

	echo "notarized"
	echo "zipping notarized..."

	ditto -c -k --sequesterRsrc --keepParent "${WRAPPER_NAME}" "${POST_NOTARIZED_ZIP}"

	echo "done. ${POST_NOTARIZED_ZIP} contains notarized "${WRAPPER_NAME}" build."
fi