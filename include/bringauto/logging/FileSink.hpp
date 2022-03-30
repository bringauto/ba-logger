#pragma once

#include <bringauto/logging/Sink.hpp>
#include <bringauto/logging/Logger.hpp>

#include <string>
#include <filesystem>
#include <optional>
#include <sys/stat.h>
#include <unistd.h>

namespace bringauto::logging {
    /**
     * Specific sink for logging into files
     */
    class FileSink : public Sink {
    public:
        /**
         * Specific file sink parameters
         */
        struct Params {

            /**
             * Constructor, file sink does need file name (without extension) and directory to save them
             */
            Params(const std::filesystem::path &logDirectoryPath, const std::string &logFileName) : fileName(
                    logFileName), fileDir(logDirectoryPath) {
                throwIfParamsNotValid(logDirectoryPath, logFileName);
            };

            const std::string fileName{};               ///log file name
            const std::filesystem::path fileDir{};      /// directory path for log files
            unsigned long maxFileSize{1024 * 50};       /// maximum size of one log file, in !bytes!
            /// number of files that can be created, one file will be created and additional numberOfRotatedFiles will be created to rotate
            unsigned int numberOfRotatedFiles{5};
            std::optional<Logger::Verbosity> verbosity; /// verbosity specific for sink, overrides default logger verbosity
        private:
            /**
             * Checking validity of log directory and log name, throws std::invalid_argument if arguments are invalid
             * @param ipv4String IPv4 string to check
             * @param serverPort port value to check
             */
            static void throwIfParamsNotValid(const std::filesystem::path &logDirectoryPath, const std::string &logFileName) {
                if (!is_directory(logDirectoryPath)) {
                    throw std::invalid_argument(logDirectoryPath.string() + " is not a directory");
                }
                if (!isDirWritable(logDirectoryPath)) {
                    throw std::invalid_argument(logDirectoryPath.string() + " is not a writable, please grant correct access rights");
                }
                if (!isFilenameCorrect(logFileName)) {
                    throw std::invalid_argument(logFileName + " contains invalid symbols.");
                }

            }

            static bool isDirWritable(const std::filesystem::path &dir){
                const int groupBufferSize = 1024;
                struct stat buf{};
                stat(dir.c_str(), &buf);
                auto ownerId = buf.st_uid;
                auto groupID = buf.st_gid;
                auto myId = getuid();

                auto permissions = std::filesystem::status(dir).permissions();
                bool ownerWrite = (permissions & std::filesystem::perms::owner_write) != std::filesystem::perms::none;
                bool groupWrite = (permissions & std::filesystem::perms::group_write) != std::filesystem::perms::none;
                bool otherWrite = (permissions & std::filesystem::perms::others_write) != std::filesystem::perms::none;

                if(otherWrite){
                    return true;
                }

                if(ownerId == myId && ownerWrite){
                    return true;
                }

                if(groupWrite){
                    int guidListSize = groupBufferSize;

                    auto groupList = std::make_unique<gid_t []>(guidListSize);
                    int numberOfGroups = getgroups(guidListSize, groupList.get());

                    for(int i = 0; i < numberOfGroups; i++){
                        if(groupList[i] == groupID){
                            return true;
                        }
                    }
                }
                return false;
            }
            static bool isFilenameCorrect(const std::string &filename){
                std::array forbiddenCharacters{'/'};
                for (const auto &character : forbiddenCharacters) {
                    if (filename.find(character) != std::string::npos) {
                        return false;
                    }
                }
                return true;
            }
        };

        /**
         * Constructor
         * @param params file specific parameters
         */
        FileSink(const Params &params) : params_(params) {};

        /**
         * Specific initialization, file sink will be added to logger
         * @param logerName name of program, logger uses same name
         */
        void init(const std::string &logerName) override;

    private:
        Params params_;
    };
}
