task :default => [:release]

task :build => [:clean] do
  cd 'build'
  sh 'nmake'
  cd '..'
end

task :release => [:build, :doc] do
	cp "build/Release/VfrToCfr.dll", "dist/vfrtocfr.dll"
	cd "dist"
	sh "zip vfrtocfr.zip *.dll *.html"
end

task :doc => [:dist] do
	sh "asciidoc -f doc/avisynth.conf -o dist/vfrtocfr.html doc/vfrtocfr.txt"
end

task :dist do
	mkdir "dist" unless File.exists?("dist")
end

task :clean do
	rm_rf "dist"
	rm_rf "build/Release"
end
