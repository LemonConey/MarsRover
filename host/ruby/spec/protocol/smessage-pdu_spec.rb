require 'spec_helper'


describe SMessagePDU do
  class PBMockMessage < Protobuf::Message
    class Message < ::Protobuf::Enum
      define :Id, 2
    end
  end

  let(:pbinst) { PBMockMessage.new }

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
