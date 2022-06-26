@import Darwin;
@import IOKit;

int main(int argc, char** argv) {
  int type = 0;
  if (argc == 2) {
    type = atoi(argv[1]);
  }
  kern_return_t err;
  CFMutableDictionaryRef matching_dict = IOServiceNameMatching("PCICrash");
  io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, matching_dict);
  if (!service) {
    fprintf(stderr, "Failed to discover PCICrash service\n");
    return 1;
  }
  fprintf(stderr, "let's try open %d\n", type);
  io_connect_t connection;
  err = IOServiceOpen(service, mach_task_self_, type, &connection);
  if (err) {
    fprintf(stderr, "IOServiceOpen failed with error: %s\n", mach_error_string(err));
    return 1;
  }
  return 0;
}
