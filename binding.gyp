{
    "targets": [
        {
            "target_name": "NativeExtension",
            "sources": [ "src/heapdump-sample.cc" ],
            "include_dirs" : [
              "<!(node -e \"require('nan')\")"
            ]
        }
    ],
}
