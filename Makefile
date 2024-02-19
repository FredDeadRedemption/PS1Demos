hello_tester:
	$(MAKE) -C hello_tester

clean:
	$(MAKE) -C hello_tester clean


	
# declare phony rules
.PHONY: hello_tester \
		clean all
