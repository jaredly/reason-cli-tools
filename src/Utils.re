include [%matchenv
  switch BSB_BACKEND {
  | "bytecode" => Utils_native
  | "native" => Utils_native
  | "js" => Utils_js
  }
];
