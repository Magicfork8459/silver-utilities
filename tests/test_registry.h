#pragma once

#include <boost/test/unit_test.hpp>

#include <silver/utilities/registry.h>

using namespace silver::utilities;

struct fixture
{
    fixture()
    {

    }

    ~fixture()
    {

    }

    registry the_registry;
};