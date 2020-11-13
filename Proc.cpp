//
// Created by JacquesdeH on 2020/11/11.
//

#include <iostream>
#include "Proc.h"

inter::Proc::Proc(const bool &_isBlockForm)
{
    this->isBlockForm = _isBlockForm;
}

void inter::Proc::addQuad(const inter::Quad &_quad)
{
    if (isBlockForm)
        std::cerr << "Should not modify Proc contents with block form !" << std::endl;
    lines.push_back(_quad);
}
