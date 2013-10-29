This library contains C++ API for pico]OS pico & nano layers. It is
mostly implemented using inline methods to achieve small code size
(compiler optimizes the result in most cases to direct pico]OS C-api call).

API has been divided into separate ::pos and ::nos namespaces.
Class names correspond to functionality groups available in
C-api, ie. posTaskCreate becomes method called "create" in class "Task" 
in namespace "pos".

