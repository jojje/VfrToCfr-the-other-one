task :default => [:package]

task :package => [:doc] do
  version = File.read('build/VfrToCfr.rc').split("\n").
                 grep(/define VERSION/).first.split.last.gsub('"','')

  %w[ dist dist/32bit dist/64bit dist/test ].each do |dir|
    mkdir dir unless File.exists?(dir)
  end

	cp "build/Release/VfrToCfr.dll", "dist/32bit/vfrtocfr.dll"
	cp "build/x64/Release/VfrToCfr.dll", "dist/64bit/vfrtocfr.dll"
	cp "test/test.avs", "dist/test/"
	cp "test/test.vpy", "dist/test/"
	cp "test/test.mp4", "dist/test/"
	cp "README.md", "dist/"
	
	cd "dist"
	sh "zip vfrtocfr-#{version}.zip -r 32bit 64bit test *.html *.md"
end

task :doc => [:dirs] do
	sh "asciidoc -f doc/avisynth.conf -o dist/vfrtocfr.html doc/vfrtocfr.txt"
end

task :dirs do
  %w[ dist dist/32bit dist/64bit dist/test ].each do |dir|
    mkdir dir unless File.exists?(dir)
  end
end

task :clean do
  %w[dist build/Release build/Debug build/x64].each do |dir|
	  rm_rf dir
	end
end
