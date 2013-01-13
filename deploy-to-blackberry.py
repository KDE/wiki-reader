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

if [ $# -ne 4 ]
then
  echo "Usage: `basename $0` <testapp> <playbook_ipaddr> <playbook_password> [8bpp | 32bpp]"
  exit -1
fi

cp ../../TouchControlOverlay/Device-Debug/libTouchControlOverlay.so ../playbook_prefix/lib/
echo "<qnx>" > blackberry-10-devalpha.xml
echo "   <id>org.libsdl.$1</id>" >> blackberry-10-devalpha.xml
echo "   <name>$1</name>" >> blackberry-10-devalpha.xml
echo "   <category>core.all</category>" >> blackberry-10-devalpha.xml
echo "   <versionNumber>1.0.0</versionNumber>" >> blackberry-10-devalpha.xml
echo "   <buildId>1</buildId>" >> blackberry-10-devalpha.xml
echo "   <description>Simple DirectMedia $1 App</description>" >> blackberry-10-devalpha.xml
echo "   <filename>$1</filename>" >> blackberry-10-devalpha.xml
echo "   <initialWindow>" >> blackberry-10-devalpha.xml
echo "      <systemChrome>none</systemChrome>" >> blackberry-10-devalpha.xml
echo "      <transparent>false</transparent>" >> blackberry-10-devalpha.xml
echo "   </initialWindow>" >> blackberry-10-devalpha.xml

echo "   <permission system=\"true\">run_native</permission>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"../playbook_prefix/lib/libSDL-1.2.so.11\" type=\"Qnx/Elf\">libSDL-1.2.so.11</asset>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"../playbook_prefix/lib/libTouchControlOverlay.so\" type=\"Qnx/Elf\">libTouchControlOverlay.so</asset>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"icon.bmp\">icon.bmp</asset>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"sail.bmp\">sail.bmp</asset>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"sample.bmp\">sample.bmp</asset>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"sample.wav\">sample.wav</asset>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"moose.dat\">moose.dat</asset>" >> blackberry-10-devalpha.xml
echo "   <asset path=\"utf8.txt\">utf8.txt</asset>" >> blackberry-10-devalpha.xml

if [ "$4" = "8bpp" ]
then
  echo "   <env var=\"SDL_VIDEODRIVER\" value=\"pb-8bit\"/>" >> blackberry-10-devalpha.xml
fi
echo "   <env var=\"LD_LIBRARY_PATH\" value=\"app/native/lib\"/>" >> blackberry-10-devalpha.xml
echo "</qnx>" >> blackberry-10-devalpha.xml

blackberry-nativepackager -package -target bar $1.bar blackberry-10-devalpha.xml $1 icon.bmp sail.bmp sample.bmp sample.wav moose.dat utf8.txt -C ../playbook_prefix ../playbook_prefix/lib/libSDL-1.2.so.11 ../playbook_prefix/lib/libTouchControlOverlay.so
blackberry-deploy -installApp -device $2 -password $3 -package $1.bar

rm $1.bar blackberry-10-devalpha.xml


