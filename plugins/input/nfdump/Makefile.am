ACLOCAL_AMFLAGS = -I m4

pluginsdir = $(datadir)/ipfixcol/plugins

plugins_LTLIBRARIES = ipfixcol-nfdump-input.la
ipfixcol_nfdump_input_la_LDFLAGS = -module -avoid-version -shared
ipfixcol_nfdump_input_la_SOURCES = nfinput.c ext_fill.c ext_fill.h ext_parse.c ext_parse.h nffile.h nfinput.h

MANSRC = ipfixcol-nfdump-input.dbk
EXTRA_DIST = $(MANSRC)
man_MANS = ipfixcol-nfdump-input.1
CLEANFILES = ipfixcol-nfdump-input.1


rpmspec = $(PACKAGE_TARNAME).spec
RPMDIR = RPMBUILD

%.1 : %.dbk
	@if [ -n "$(XSLTPROC)" ]; then \
		if [ -f "$(XSLTMANSTYLE)" ]; then \
			echo $(XSLTPROC) $(XSLTMANSTYLE) $<; \
			$(XSLTPROC) $(XSLTMANSTYLE) $<; \
		else \
			echo "Missing $(XSLTMANSTYLE)!"; \
			exit 1; \
		fi \
	else \
		echo "Missing xsltproc"; \
	fi


.PHONY: rpm
rpm: dist $(rpmspec)
	@mkdir -p $(RPMDIR)/BUILD $(RPMDIR)/RPMS $(RPMDIR)/SOURCES $(RPMDIR)/SPECS $(RPMDIR)/SRPMS;
	mv $(PACKAGE_TARNAME)-$(PACKAGE_VERSION).tar.gz $(RPMDIR)/SOURCES/$(PACKAGE_TARNAME)-$(PACKAGE_VERSION)-$(RELEASE).tar.gz
	$(RPMBUILD) -ba $(rpmspec) \
		--define "_topdir `pwd`/$(RPMDIR)";

clean-local: 
	rm -rf RPMBUILD