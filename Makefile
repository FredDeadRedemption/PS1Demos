snake_demo:
	$(MAKE) -C snake_demo

clean:
	$(MAKE) -C snake_demo clean


	
# declare phony rules
.PHONY: snake_demo \
		clean all
