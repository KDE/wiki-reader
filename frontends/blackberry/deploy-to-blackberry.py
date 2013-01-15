 # This file is part of the WikiReader project
 # Copyright (c) 2013 Laszlo Papp <lpapp@kde.org>
 #
 # This library is free software; you can redistribute it and/or
 # modify it under the terms of the GNU Lesser General Public
 # License as published by the Free Software Foundation; either
 # version 2.1 of the License, or (at your option) any later version.
 #
 # This library is distributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 # Lesser General Public License for more details.
 #
 # You should have received a copy of the GNU Lesser General Public
 # License along with this library; if not, write to the Free Software
 # Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 #

import argparse
import sys
import shlex
import subprocess

def main():
    parser = argparse.ArgumentParser(description='A helper script to deploy your application to a blackberry device')

    parser.add_argument("--ipaddress", help="Use the selected ip address and relevant device")
    parser.add_argument("--password", help="Use the selected password for device authorization")
    parser.add_argument("--bar-descriptor-xml-path", help="Use the selected path for the bar descriptor file")
    parser.add_argument("--package-name", help="Use the selected package name for creating the bar file")

    args = parser.parse_args()

    subprocess.call(["blackberry-nativepackager", "-package", "-target", "bar", "%s.bar" % args.package_name, args.bar_descriptor_xml_path])
    subprocess.call(["blackberry-deploy", "-installApp", "-device", args.ipaddress, "-password", args.password, "-package", "%s.bar" % args.package_name])

if __name__ == "__main__":
    sys.exit(main())
