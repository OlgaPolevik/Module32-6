//
//  main.cpp
//  Module32-6
//
//  Created by Ольга Полевик on 22.01.2022.
//


//=============================================
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace std;

class DBConnection
{
public:
    virtual ~DBConnection() {}
    
    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool execQuery() = 0;
};

class DBConnectionMock : public DBConnection
{
public:
    MOCK_METHOD(bool, open,(), (override) );
    MOCK_METHOD(bool, close,(), (override) );
    MOCK_METHOD(bool, execQuery,(), (override) );
};

class ClassThatUsesDB
{
public:
    virtual ~ClassThatUsesDB() {}
    
    virtual bool openConnection() = 0;
    virtual bool useConnection() = 0;
    virtual bool closeConnection() = 0;
};

class ClassThatUsesDBImpl : public ClassThatUsesDB
{
public:
    ClassThatUsesDBImpl( DBConnection* connection )
        : connection( connection )
    {
    }
    
public:
    virtual bool openConnection() override
    {
        return connection->open();
        
    }
    virtual bool useConnection() override
    {
        return connection->execQuery();
    }
    virtual bool closeConnection() override
    {
        return connection->close();
    }
    
private:
    DBConnection *connection;
};

class SomeTestSuite : public ::testing::Test
{
protected:
   void SetUp()
   {
       mock = new DBConnectionMock ();
       obj = new ClassThatUsesDBImpl(mock);
   }

   void TearDown()
   {
       delete obj;
       delete mock;
   }

protected:
    ClassThatUsesDB *obj;
    DBConnectionMock *mock;
};


TEST_F(SomeTestSuite, testcase_open)
{
    EXPECT_CALL(*mock, open()).WillOnce(::testing::Return(true));
    ASSERT_EQ(obj->openConnection(), true);
}

TEST_F(SomeTestSuite, testcase_execQuery)
{
    EXPECT_CALL(*mock, execQuery()).WillOnce(::testing::Return(true));
    ASSERT_EQ(obj->useConnection(), true);
}

TEST_F(SomeTestSuite, testcase_close)
{
    EXPECT_CALL(*mock, close()).WillOnce(::testing::Return(true));
    ASSERT_EQ(obj->closeConnection(), true);
}



int main(int argc, char *argv[])
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
