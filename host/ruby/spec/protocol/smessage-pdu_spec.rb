require 'spec_helper'

describe SMessagePDU do

  class PBMockMessage < Protobuf::Message
    class Message < ::Protobuf::Enum
      define :Id, 5
    end
  end

  let(:pbinst) { PBMockMessage.new }

  context 'serialize' do

    before(:each) do
      pbinst.stub(:serialize).and_return("hello world")
    end

    it 'should raise exception if message is not the protobuf message instance' do
      expect {
        fakePBObj = Object.new
        SMessagePDU.serialize(fakePBObj)
      }.to raise_error
    end

    it 'should serialize protobuf message with smessage header' do
      buffer = SMessagePDU.serialize pbinst
      buffer.size.should == "hello world".size + 2
      buffer.bytes[0].should == pbinst.class::Message::Id
      buffer.bytes[1].should == "hello world".size
    end
  end

  context 'parse' do

    it 'should generate protobuf message instance by id' do
      # call new once 
      pbinst

      PBMockMessage.should_receive(:new).and_return(pbinst)
      pbinst.should_receive(:parse_from_string).with("hehe")

      SMessagePDU.parse("\5\4hehe").should == pbinst
    end

    it 'should raise TypeError if no matched protobuf message is found' do
      expect {
        ap SMessagePDU.parse("\377\4hehe").class
      }.to raise_error(TypeError)
    end

    it 'should raise RangeError if datasize in SMessagePDU doesnt match the payload size' do
      expect {
        SMessagePDU.parse("\5\5hehe")
      }.to raise_error(RangeError)
    end

  end
end
