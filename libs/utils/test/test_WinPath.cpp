/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <limits.h>
#include <gtest/gtest.h>

#include <utils/Path.h>

#include <iosfwd>
#include <string>
#include <vector>

#ifndef PATH_MAX    // should be in <limits.h>
#define PATH_MAX    4096
#endif

using namespace utils;

/*
TEST(PathTest, Concatenate) {
    Path root("/Volumes/Replicant/blue");

    Path r;
    r = root.concat("");
    EXPECT_EQ("/Volumes/Replicant/blue", r.getPath());

    r = root.concat("/out/bin");
    EXPECT_EQ("/out/bin", r.getPath());

    r = root.concat("out/bin");
    EXPECT_EQ("/Volumes/Replicant/blue/out/bin", r.getPath());

    r = root.concat(".");
    EXPECT_EQ("/Volumes/Replicant/blue", r.getPath());

    r = root.concat("..");
    EXPECT_EQ("/Volumes/Replicant", r.getPath());

    r = root.concat("/");
    EXPECT_EQ("/", r.getPath());

    r = root.concat("../remote-blue");
    EXPECT_EQ("/Volumes/Replicant/remote-blue", r.getPath());

    r = root.concat("../remote-blue");
    EXPECT_EQ(r, root + Path("../remote-blue"));
    EXPECT_EQ(r, root + "../remote-blue");

    r = "/out/bin";
    r.concatToSelf("../bin");
    EXPECT_EQ("/out/bin", r.getPath());

    r += "./resources";
    EXPECT_EQ("/out/bin/resources", r.getPath());
}
*/

TEST(PathTest, Sanitization) {
    std::string r, s;

    // An empty path remains empty
    r = Path::getCanonicalPath("");
    EXPECT_EQ("", r);

    // A single / is preserved
	/*
    r = Path::getCanonicalPath("\\");
    EXPECT_EQ("\\", r);

    // A single / is preserved
    r = Path::getCanonicalPath("\\\\");
    EXPECT_EQ("\\", r);
	*/

    // A leading . is preserved
    r = Path::getCanonicalPath(".\\out");
	s = Path::getCanonicalPath("out");
    EXPECT_EQ(s, r);

    r = Path::getCanonicalPath("..\\..\\out\\..\\foo");
	s = Path::getCanonicalPath("..\\..\\foo");
	EXPECT_EQ(r, s);

    // A leading . is preserved
	/*
    r = Path::getCanonicalPath(".");
    EXPECT_EQ(".", r);
	*/

    // A leading .. is preserved
	/*
    r = Path::getCanonicalPath("\\.");
    EXPECT_EQ("\\", r);
	*/

	// todo: add test for drive

    // A leading .. is preserved
	/*
    r = Path::getCanonicalPath("..\\out");
    EXPECT_EQ("..\\out", r);
	*/

    // A leading .. is preserved
	/*
    r = Path::getCanonicalPath("\\..");
    EXPECT_EQ("\\", r);
	*/

    // A leading \\ is preserved
	/*
    r = Path::getCanonicalPath("\\out");
    EXPECT_EQ("\\out", r);
	*/

    // A middle . is removed
    r = Path::getCanonicalPath("out\\.\\bin");
    s = Path::getCanonicalPath("out\\bin");
    EXPECT_EQ(s, r);

    // two middle . are removed
    r = Path::getCanonicalPath("out\\.\\.\\bin");
    s = Path::getCanonicalPath("out\\bin");
    EXPECT_EQ(s, r);

    // three middle . are removed
    r = Path::getCanonicalPath("out\\.\\.\\.\\bin");
    s = Path::getCanonicalPath("out\\bin");
    EXPECT_EQ(s, r);

    // a starting . is kept
    r = Path::getCanonicalPath(".\\bin");
    s = Path::getCanonicalPath("bin");
    EXPECT_EQ(s, r);

    // several starting . are collapsed to one
    r = Path::getCanonicalPath(".\\.\\bin");
    s = Path::getCanonicalPath(".\\bin");
    EXPECT_EQ(s, r);

    // several starting . are collapsed to one
    r = Path::getCanonicalPath(".\\.\\.\\bin");
    s = Path::getCanonicalPath(".\\bin");
    EXPECT_EQ(s, r);

    // A middle .. is removed and the previous segment is removed
    r = Path::getCanonicalPath("out\\blue\\..\\bin");
	s = Path::getCanonicalPath("out\\bin");
    EXPECT_EQ(s, r);

    // Special case of the previous test
    // A .. in second spot pops to an empty stack
    r = Path::getCanonicalPath("out\\..\\bin");
	s = Path::getCanonicalPath("bin");
    EXPECT_EQ(s, r);

    // Special case of the previous test
    // A .. in second spot pops to an empty stack
    r = Path::getCanonicalPath("out\\..\\..\\bin");
    s = Path::getCanonicalPath("..\\bin");
    EXPECT_EQ(s, r);

    // make sure it works with several ../
	/*
    r = Path::getCanonicalPath("..\\..\\bin");
    EXPECT_EQ("..\\..\\bin", r);
	*/

    // make sure to test odd/even numbers of ../ and more than one
	/*
    r = Path::getCanonicalPath("..\\..\\..\\bin");
    EXPECT_EQ("..\\..\\..\\bin", r);
	*/

    // check odd and more than 1 or 2 ../ in the middle
    r = Path::getCanonicalPath("out\\..\\..\\..\\bin");
    s = Path::getCanonicalPath("..\\..\\bin");
    EXPECT_EQ(s, r);

    // Two or more slashes is the same as one
    r = Path::getCanonicalPath("out\\blue\\\\bin");
    s = Path::getCanonicalPath("out\\blue\\bin");
    EXPECT_EQ(s, r);

    // A trailing / is preserved
	/*
    r = Path::getCanonicalPath("out\\blue\\bin\\");
    EXPECT_EQ("out\\blue\\bin\\", r);
	*/

    // Both leading and trailing / are preserved
	/*
    r = Path::getCanonicalPath("\\out\\blue\\bin\\");
    EXPECT_EQ("\\out\\blue\\bin\\", r);
	*/

    // preserve a segment starting with a .
	/*
    r = Path::getCanonicalPath("\\out\\.blue\\bin\\");
    EXPECT_EQ("\\out\\.blue\\bin\\", r);
	*/

    // remove a /./ following a ..
	/*
    r = Path::getCanonicalPath("\\out\\..\\.\\bin\\");
    EXPECT_EQ("\\bin\\", r);
	*/

    // remove a /./ following a ..
    r = Path::getCanonicalPath("..\\.\\bin\\");
    s = Path::getCanonicalPath("..\\bin\\");
    EXPECT_EQ(s, r);

    // collapse multiple /
	/*/
    r = Path::getCanonicalPath("\\\\\\\\");
    EXPECT_EQ("\\", r);
	*/

    // collapse multiple /
	/*
    r = Path::getCanonicalPath("\\aaa\\\\\\bbb\\");
    EXPECT_EQ("\\aaa\\bbb\\", r);
	*/

    // collapse multiple /
	/*
    r = Path::getCanonicalPath("\\\\\\.\\\\\\");
    EXPECT_EQ("\\", r);
	*/

    // multiple ..
    r = Path::getCanonicalPath("..\\out\\..\\in");
    s = Path::getCanonicalPath("..\\in");
    EXPECT_EQ(s, r);

    // /..
	/*/
    r = Path::getCanonicalPath("\\..\\out\\..\\in");
    EXPECT_EQ("\\in", r);
	*/

    // No sanitizing required
	/*
    r = Path::getCanonicalPath("out");
    EXPECT_EQ("out", r);
	*/
}

/*/
TEST(PathTest, GetParent) {
    std::string r;
    Path p("/out/bin");
    r = p.getParent();
    EXPECT_EQ("/out/", r);

    p = "/out/bin/";
    r = p.getParent();
    EXPECT_EQ("/out/", r);

    p = "out/bin";
    r = p.getParent();
    EXPECT_EQ("out/", r);

    p = "out/bin/";
    r = p.getParent();
    EXPECT_EQ("out/", r);

    p = "out";
    r = p.getParent();
    EXPECT_EQ("", r);

    p = "/out";
    r = p.getParent();
    EXPECT_EQ("/", r);

    p = "";
    r = p.getParent();
    EXPECT_EQ("", r);

    p = "/";
    r = p.getParent();
    EXPECT_EQ("/", r);
}
*/

/*
TEST(PathTest, GetName) {
    Path p("/out/bin");
    EXPECT_EQ("bin", p.getName());

    p = "/out/bin/";
    EXPECT_EQ("bin", p.getName());

    p = "/";
    EXPECT_EQ("/", p.getName());

    p = "out";
    EXPECT_EQ("out", p.getName());
}
*/

/*
TEST(PathTest, Split) {
    std::vector<std::string> segments;

    segments = Path("").split();
    EXPECT_EQ(0, segments.size());

    segments = Path("/").split();
    EXPECT_EQ(1, segments.size());
    EXPECT_EQ("/", segments[0]);

    segments = Path("out/blue/bin").split();
    EXPECT_EQ(3, segments.size());
    EXPECT_EQ("out", segments[0]);
    EXPECT_EQ("blue", segments[1]);
    EXPECT_EQ("bin", segments[2]);

    segments = Path("/out/blue/bin").split();
    EXPECT_EQ(4, segments.size());
    EXPECT_EQ("/", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("blue", segments[2]);
    EXPECT_EQ("bin", segments[3]);

    segments = Path("/out/blue/bin/").split();
    EXPECT_EQ(4, segments.size());
    EXPECT_EQ("/", segments[0]);
    EXPECT_EQ("out", segments[1]);
    EXPECT_EQ("blue", segments[2]);
    EXPECT_EQ("bin", segments[3]);
}
*/

/*
TEST(PathTest, CurrentDirectory) {
    Path p(Path::getCurrentDirectory());
    EXPECT_FALSE(p.isEmpty());
    EXPECT_TRUE (p.isAbsolute());
    EXPECT_TRUE (p.exists());
    EXPECT_TRUE (p.isDirectory());
    EXPECT_FALSE(p.isFile());
}
*/

/*
TEST(PathTest, CurrentExecutable) {
    Path p(Path::getCurrentExecutable());
    EXPECT_FALSE(p.isEmpty());
    EXPECT_TRUE(p.isAbsolute());
    EXPECT_TRUE(p.exists());
    EXPECT_FALSE(p.isDirectory());
    EXPECT_TRUE (p.isFile());
}
*/

/*
TEST(PathTest, AbsolutePath) {
    Path cwd = Path::getCurrentDirectory();

    Path p;
    p = Path("/out/blue/bin");
    EXPECT_TRUE(p.isAbsolute());

    p = p.getAbsolutePath();
    EXPECT_EQ("/out/blue/bin", p.getPath());

    p = Path("../bin").getAbsolutePath();
    EXPECT_NE(cwd, p);
    EXPECT_TRUE(p.isAbsolute());
}
*/

/*
TEST(PathTest, GetExtension) {
    Path p("/out/bin/somefile.txt");
    EXPECT_EQ(p.getExtension(), "txt");

    p = Path("/out/bin/somefilewithoutextension");
    EXPECT_EQ(p.getExtension(), "");

    p = Path("/out/bin/.tempdir/somefile.txt.bak");
    EXPECT_EQ(p.getExtension(), "bak");

    p = Path("/out/bin/.tempfile");
    EXPECT_EQ(p.getExtension(), "");

    p = Path("/out/bin/endsindot.");
    EXPECT_EQ(p.getExtension(), "");

    p = Path::getCurrentDirectory();
    EXPECT_EQ(p.getExtension(), "");

    p = Path();
    EXPECT_EQ(p.getExtension(), "");
}
*/
