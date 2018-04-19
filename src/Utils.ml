#if BSB_BACKEND = "bytecode" || BSB_BACKEND = "native" then
  include Utils_native
#else 
  include Utils_js
#end
