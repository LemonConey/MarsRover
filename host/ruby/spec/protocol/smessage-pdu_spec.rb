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

    it 'should return nil if message is nt the protobuf message instance' do
      fakePBObj = Object.new
      SMessagePDU.serialize(fakePBObj).should == nil
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
    
    it 'should return nil if no matched protobuf message is found' do
      SMessagePDU.parse("\0xfe\4hehe").should == nil
    end
    
  end
end
