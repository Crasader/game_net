@echo off

echo ��ʼ����proto��Server
copy game.proto ..\..\Server\Server\

echo ���뵽Server���ڵ�Ŀ¼
cd ..\..\Server\Server\

echo ��ʼ����.h��.cc
protoc --cpp_out ./ game.proto

echo ������ɣ��ټ�!
pause
exit