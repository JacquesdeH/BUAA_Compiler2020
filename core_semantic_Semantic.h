//
// Created by JacquesdeH on 2020/11/11.
//

#ifndef PROJECT_CORE_SEMANTIC_SEMANTIC_H
#define PROJECT_CORE_SEMANTIC_SEMANTIC_H

#include <string>

#include "utils_inter_MIR.h"

namespace semantic
{
    class Semantic
    {
    private:
        bool errored;
        int tempIdx;
        int labelIdx;
        int stringIdx;
        inter::MIR mir;
        std::vector<inter::Quad> _recorded;
        bool _isRecording;
        bool _actualAdding;
        std::string exitLabel;

    public:
        Semantic();

    public:
        void error();
        bool noError() const;
        std::string genTemp();
        std::string genLabel();
        std::string genGlobalString(const std::string & _strContent);
        std::string genGlobalChar(const std::string & _name, const int & _count, const std::vector<char> & _initValues = {});
        std::string genGlobalInt(const std::string & _name, const int & _count, const std::vector<int> & _initValues = {});
        std::string genLocalChar(const std::string & _name, const int & _count, const std::vector<char> & _initValues = {});
        std::string genLocalInt(const std::string & _name, const int & _count, const  std::vector<int> & _initValues = {});
        std::string generateExtended(const std::string & _name, const std::string & _type);
        void addMIR(const config::IRCode & _op, const std::string & _out = "", const std::string & _inl = "", const std::string & _inr = "");
        void addBranch(const config::TokenCode &_operator, const std::string &_exprL, const std::string &_exprR,
                       const std::string &_label, const bool &_branchCondition);
        void setLabel(const std::string &_label);
        void newProc(const std::string & _procName);
        void addParam(const std::string &_name, const std::string &_type);
        void doneGenerationToBlocks();
        void startRecording(const bool &_actual = true);
        std::vector<inter::Quad> endRecording();
        void addRecord(const std::vector<inter::Quad> &_record);
        void addSwitch(const std::string &tempSwitch,
                       const std::vector<std::pair<int, std::vector<inter::Quad> > > &subCaseRecordAll,
                       const std::vector<inter::Quad> &defaultRecord);
        const inter::MIR &getMir() const;
        void enterMainGenLabelExit();
        std::string queryExitLabel();
        void exitMainResetLabelExit();
    };
}


#endif //PROJECT_CORE_SEMANTIC_SEMANTIC_H
