target "x86_64" {
    platforms = ["linux/amd64"]
    args = {
        "image_name": "x86_64-ubuntu_2004",
        "nap_linux_arch": "x86_64"
    }
    output = ["./linux/x86_64"]
}

target "armhf" {
    platforms = ["linux/armv6"]
    args = {
        "image_name": "armhf-raspbian_buster",
        "nap_linux_arch": "armhf"
    }
    output = ["./linux/armhf"]
}

target "arm64" {
    platforms = ["linux/arm64/v8"]
    args = {
        "image_name": "arm64-ubuntu_2004",
        "nap_linux_arch": "arm64"
    }
    output = ["./linux/arm64"]
}
