//
// Created by JacquesdeH on 2020/11/11.
//

#include "core_semantic_Semantic.h"
#include "functional_strext.h"

semantic::Semantic::Semantic()
{
    this->errored = false;
    this->tempIdx = 0;
    this->labelIdx = 0;
    this->stringIdx = 0;
    this->_isRecording = false;
    this->_recorded.clear();
}

void semantic::Semantic::error()
{
    this->errored = true;
}

bool semantic::Semantic::noError() const
{
    return !(this->errored);
}

void semantic::Semantic::addMIR(const config::IRCode &_op, const std::string &_out, const std::string &_inl,
                                const std::string &_inr)
{
    if (this->errored)
        return;
    inter::Quad tmp(_op, _out, _inl, _inr);
    if (!_isRecording || (_isRecording && _actualAdding))
        mir.addQuad(tmp);
    if (this->_isRecording)
        this->_recorded.push_back(tmp);
}

std::string semantic::Semantic::genTemp()
{
    tempIdx++;
    return config::tempHead + toString(tempIdx);
}

std::string semantic::Semantic::genLabel()
{
    labelIdx++;
    return config::labelHead + toString(labelIdx);
}

std::string semantic::Semantic::genGlobalString(const std::string & _strContent)
{
    stringIdx++;
    string name = config::stringHead + toString(stringIdx);
    mir.declareGlobalString(name, _strContent);
    return name;
}

void semantic::Semantic::newProc(const std::string & _procName)
{
    mir.newProc(config::procHead + toLower(_procName));
}

void semantic::Semantic::doneGenerationToBlocks()
{
    mir.doneGenerationToBlocks();
}

std::string
semantic::Semantic::genGlobalChar(const string &_name, const int &_count, const std::vector<char> &_initValues)
{
    string _mark = config::globalHead + toLower(_name);
    this->mir.declareGlobalChar(_mark, _count, _initValues);
    return _mark;
}

std::string semantic::Semantic::genGlobalInt(const string &_name, const int &_count, const std::vector<int> &_initValues)
{
    string _mark = config::globalHead + toLower(_name);
    this->mir.declareGlobalInt(_mark, _count, _initValues);
    return _mark;
}

const inter::MIR &semantic::Semantic::getMir() const
{
    return mir;
}

std::string
semantic::Semantic::genLocalChar(const string &_name, const int &_count, const std::vector<char> &_initValues)
{
    string _mark = config::localHead + toLower(_name);
    this->mir.declareLocalChar(_mark, std::make_pair(_count, _initValues));
    return _mark;
}

std::string semantic::Semantic::genLocalInt(const string &_name, const int &_count, const std::vector<int> &_initValues)
{
    string _mark = config::localHead + toLower(_name);
    this->mir.declareLocalInt(_mark, std::make_pair(_count, _initValues));
    return _mark;
}

std::string semantic::Semantic::generateExtended(const string &_name, const string &_type)
{
    std::string lowerName = toLower(_name);
    if (_type == "global")
        return config::globalHead + lowerName;
    else if (_type == "local")
        return config::localHead + lowerName;
    else if (_type == "proc")
        return config::procHead + lowerName;
    else if (_type == "temp")
        return config::tempHead + lowerName;
    else if (_type == "label")
        return config::labelHead + lowerName;
    else if (_type == "string")
        return config::stringHead + lowerName;
    return lowerName;
}

void semantic::Semantic::addBranch(const config::TokenCode &_operator, const string &_exprL, const string &_exprR,
                                   const string &_label, const bool &_branchCondition)
{
    if (this->errored)
        return;
    config::TokenCode _finetunedOp;
    if (_branchCondition == true)
        _finetunedOp = _operator;
    else
        switch (_operator)
        {
            case config::EQL:
                _finetunedOp = config::NEQ;
                break;
            case config::NEQ:
                _finetunedOp = config::EQL;
                break;
            case config::LEQ:
                _finetunedOp = config::GRE;
                break;
            case config::LSS:
                _finetunedOp = config::GEQ;
                break;
            case config::GRE:
                _finetunedOp = config::LEQ;
                break;
            case config::GEQ:
                _finetunedOp = config::LSS;
                break;
            default:
                _finetunedOp = config::EMPTY;
        }
    // branch on true condition now
    switch (_finetunedOp)
    {
        case config::EQL:
            addMIR(config::BEQ_IR, _label, _exprL, _exprR);
            break;
        case config::NEQ:
            addMIR(config::BNE_IR, _label, _exprL, _exprR);
            break;
        case config::LEQ:
            addMIR(config::BLE_IR, _label, _exprL, _exprR);
            break;
        case config::LSS:
            addMIR(config::BLT_IR, _label, _exprL, _exprR);
            break;
        case config::GRE:
            addMIR(config::BLT_IR, _label, _exprR, _exprL);
            break;
        case config::GEQ:
            addMIR(config::BLE_IR, _label, _exprR, _exprL);
            break;
        default:
            std::cerr << "Unexpected finetuned operator in addBranch !" << std::endl;
    }
}

void semantic::Semantic::setLabel(const string &_label)
{
    if (this->errored)
        return;
    addMIR(config::SETLABEL_IR, _label);
}

void semantic::Semantic::startRecording(const bool &_actual)
{
    if (this->errored)
        return;
    if (this->_isRecording)
        std::cerr << "Already has been recording !" << std::endl;
    this->_isRecording = true;
    this->_actualAdding = _actual;
}

std::vector<inter::Quad> semantic::Semantic::endRecording()
{
    if (this->errored)
        return std::vector<inter::Quad>();
    if (!(this->_isRecording))
        std::cerr << "Not recording when terminating records !" << std::endl;
    this->_isRecording = false;
    this->_actualAdding = true;
    return this->_recorded;
}

void semantic::Semantic::addRecord(const std::vector<inter::Quad> &_record)
{
    if (this->errored)
        return;
    for (const auto &_quad : _record)
    {
        mir.addQuad(_quad);
    }
}

void semantic::Semantic::addParam(const string &_name, const string &_type)
{
    mir.addParam(_name, _type);

}

void semantic::Semantic::addSwitch(const std::string &tempSwitch,
                                   const std::vector<std::pair<int, std::vector<inter::Quad> > > &subCaseRecordAll,
                                   const std::vector<inter::Quad> &defaultRecord)
{
    std::vector<std::string> branchLabels;
    for (const auto &_entry : subCaseRecordAll)
    {
        const int &_key = _entry.first;
        const std::vector<inter::Quad> &_record = _entry.second;
        std::string _label = this->genLabel();
        this->addBranch(config::EQL, tempSwitch, toString(_key), _label, true);
        branchLabels.push_back(_label);
    }
    std::string exitLabel = this->genLabel();
    if (subCaseRecordAll.size() != branchLabels.size())
        std::cerr << "Labels and Branches count do not match !" << std::endl;
    // default record
    {
        // no enter label required
        this->addRecord(defaultRecord);
        this->addMIR(config::JUMP_IR, exitLabel);
    }
    // sub case records
    for (int i = 0; i < subCaseRecordAll.size(); ++i)
    {
        const std::string &_label = branchLabels[i];
        const std::vector<inter::Quad> &_record = subCaseRecordAll[i].second;
        this->setLabel(_label);
        this->addRecord(_record);
        this->addMIR(config::JUMP_IR, exitLabel);
    }
    // set exit label
    this->setLabel(exitLabel);
}
