#include <bringauto/logging/RemoteSink.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <SinksApiTests.hpp>



using namespace bringauto::logging;

TEST_F(SinksApiTests, RemoteSinkParameters
) {
std::vector<std::string> correctIPv4s = { "192.168.1.1", "127.0.0.1", "0.0.0.0", "255.255.255.255" };
std::vector<std::string> incorrectIPv4s = { "092.168.1.1", "aaef", "a.a.a.a", "256.1.1.1", "45.01.48.45",
											"45.45.4." };
unsigned correctPort = 42;

for(
const auto &ip
: correctIPv4s){
EXPECT_NO_THROW(RemoteSink::Params(ip, correctPort)
);
}
for(
const auto &ip
: incorrectIPv4s){
EXPECT_ANY_THROW(RemoteSink::Params(ip, correctPort)
);
}
EXPECT_NO_THROW(RemoteSink::Params("192.168.1.1", 1)
);
EXPECT_NO_THROW(RemoteSink::Params("192.168.1.1", 65535)
);

EXPECT_ANY_THROW(RemoteSink::Params("192.168.1.1", 65536)
);
EXPECT_ANY_THROW(RemoteSink::Params("192.168.1.1", 100000)
);
}

TEST_F(SinksApiTests, RemoteSinkCreation
) {
EXPECT_NO_THROW(RemoteSink(RemoteSink::Params("192.168.1.1", 56))
);
EXPECT_ANY_THROW(RemoteSink(RemoteSink::Params("192.168.1.01", 65536))
);

EXPECT_NO_THROW(RemoteSink(RemoteSink::Params("192.168.1.1", 56))
);

RemoteSink::Params params { "127.0.0.1", 56 };
params.
verbosity = Logger::Verbosity::Error;

EXPECT_NO_THROW(RemoteSink({ params })
);
}

TEST_F(SinksApiTests, ConsoleSinkCreation
) {
EXPECT_NO_THROW(ConsoleSink(ConsoleSink::Params {})
);

ConsoleSink::Params params {};
params.
verbosity = Logger::Verbosity::Debug;
EXPECT_NO_THROW(ConsoleSink({ params })
);
}

TEST_F(SinksApiTests, FileSinkParameters
){
std::filesystem::path incorrectPath { "/var/" };
std::filesystem::path correctPath { "./" };
std::string wrongFileName { "text/.me" };
std::string correctFileName { "text.me" };

if(
exists(incorrectPath)
){
if (!
hasWriteAccess(incorrectPath)
) {
EXPECT_ANY_THROW(FileSink({ incorrectPath, correctFileName })
);
}else{
std::cerr << "[warning] Program has access to /var folder, cannot test access rights." <<
std::endl;
}
}else{
std::cerr << "[warning] Unable to test existing file without access (/var does not exists)." <<
std::endl;
}

EXPECT_ANY_THROW(FileSink({ correctPath, wrongFileName })
);

std::filesystem::path nonExistingPath("/4545a484d6a841aa3413a48/da4d86a433asd*...*");
if(
exists(nonExistingPath)
){
std::cerr << "[warning] Unable to test non-existing folder." <<
std::endl;
}else{
EXPECT_ANY_THROW(FileSink({ nonExistingPath, correctFileName })
);
}
}

TEST_F(SinksApiTests, FileSinkCreation
){
EXPECT_NO_THROW(FileSink({ "./", "text" })
);

FileSink::Params params { "./", "text" };
params.
maxFileSize = 56666;
params.
numberOfRotatedFiles = 5;

EXPECT_NO_THROW(FileSink({ params })
);

}