# encoding: utf-8

#
# Advanced MultiProgressMeter example:
# Change values interactively with sliders
# and allow tests with huge numbers
#
module Yast
  class MultiProgressMeter2Client < Client
    def main
      Yast.import "UI"
      #
      # Global variables
      #

      @maxValueList = [950, 200, 500, 20, 100]
      @valueList = [100, 30, 400, 0, 0]
      @unit = 0 # exponent: powers of 2



      #
      # Check if required special widgets are available
      #

      if !UI.HasSpecialWidget(:HMultiProgressMeter) ||
          !UI.HasSpecialWidget(:Slider)
        UI.OpenDialog(
          VBox(
            Label("Error: This UI doesn't support the required widgets!"),
            PushButton(Opt(:default), "&OK")
          )
        )
        UI.UserInput
        UI.CloseDialog

        return
      end


      #
      # Create dialog
      #

      @radioBox = Frame(
        "Unit",
        RadioButtonGroup(
          Id(:unit),
          Opt(:notify),
          HBox(
            HSpacing(0.5),
            RadioButton(Id(term(:unit, 0)), Opt(:notify), "&Bytes"),
            HSpacing(1.5),
            RadioButton(Id(term(:unit, 10)), Opt(:notify), "&kB"),
            HSpacing(1.5),
            RadioButton(Id(term(:unit, 20)), Opt(:notify), "&MB"),
            HSpacing(1.5),
            RadioButton(Id(term(:unit, 30)), Opt(:notify), "&GB"),
            HSpacing(0.5)
          )
        )
      )

      UI.OpenDialog(
        VBox(
          HBox(
            VBox(
              Heading("MultiProgressMeter Example"),
              VSpacing(1),
              slidersVBox(@maxValueList, @valueList),
              @radioBox,
              VStretch()
            ),
            HSpacing(1),
            ReplacePoint(
              Id(:rep_vProgress),
              VMultiProgressMeter(
                Id(:vProgress),
                scaleList(@unit, @maxValueList)
              )
            )
          ),
          HBox(
            ReplacePoint(
              Id(:rep_hProgress),
              HMultiProgressMeter(
                Id(:hProgress),
                scaleList(@unit, @maxValueList)
              )
            ),
            HSpacing(0.5),
            PushButton(Id(:cancel), "&Close")
          )
        )
      )

      UI.ChangeWidget(Id(:unit), :Value, term(:unit, @unit))
      updateProgress


      #
      # Event loop
      #

      while true
        @event = UI.WaitForEvent
        # y2debug( "Event: %1", event );
        @id = Ops.get(@event, "ID")
        @widgetClass = Ops.get_symbol(@event, "WidgetClass")

        if @widgetClass == :RadioButton
          @currentUnitID = UI.QueryWidget(:unit, :CurrentButton)

          if Ops.is_term?(@currentUnitID)
            @unit = Ops.get_integer(Convert.to_term(@currentUnitID), 0, 0)

            Builtins.y2milestone("New unit: 2^%1", @unit)
            UI.ReplaceWidget(
              :rep_vProgress,
              VMultiProgressMeter(
                Id(:vProgress),
                scaleList(@unit, @maxValueList)
              )
            )
            UI.ReplaceWidget(
              :rep_hProgress,
              HMultiProgressMeter(
                Id(:hProgress),
                scaleList(@unit, @maxValueList)
              )
            )
            updateProgress
          end
        end
        if @widgetClass == :Slider
          updateProgress
          @values = scaleList(@unit, getValues)
          UI.ChangeWidget(:vProgress, :Values, @values)
          UI.ChangeWidget(:hProgress, :Values, @values)
        end

        break if @id == :cancel
      end


      #
      # Clean up
      #

      UI.CloseDialog

      nil
    end

    # Return a VBox term with Slider widgets for each list value.
    #
    def slidersVBox(maxValuesList, currentValuesList)
      maxValuesList = deep_copy(maxValuesList)
      currentValuesList = deep_copy(currentValuesList)
      vbox = VBox()
      i = 0

      Builtins.foreach(maxValuesList) do |maxVal|
        vbox = Builtins.add(
          vbox,
          Slider(
            Id(term(:slider, i)), # currentVal
            Opt(:notify),
            "", # label
            0, # minVal
            maxVal,
            Ops.get(currentValuesList, i, 0)
          )
        )
        i = Ops.add(i, 1)
      end

      deep_copy(vbox)
    end


    # Apply unit to a list of values. Return the scaled list.
    #
    def scaleList(unit, values)
      values = deep_copy(values)
      scaledValues = []

      Builtins.foreach(values) do |val|
        scaledValues = Builtins.add(scaledValues, Ops.shift_left(val, unit))
      end

      # y2debug( "Values: %1	  unit: %2   scaled: %3", values, unit, scaledValues );
      deep_copy(scaledValues)
    end


    # Get the current values from all sliders and return them as a list.
    #
    def getValues
      values = []
      i = 0

      while true
        sliderID = term(:slider, i)
        # y2debug( "Looking for %1", sliderID );

        break if !UI.WidgetExists(Id(sliderID))

        values = Builtins.add(
          values,
          Convert.to_integer(UI.QueryWidget(Id(sliderID), :Value))
        )
        i = Ops.add(i, 1)
      end

      # y2debug( "Values: %1", values );
      deep_copy(values)
    end


    # Update progress meters with values from sliders.
    #
    def updateProgress
      values = scaleList(@unit, getValues)
      UI.ChangeWidget(:vProgress, :Values, values)
      UI.ChangeWidget(:hProgress, :Values, values)

      nil
    end
  end
end

Yast::MultiProgressMeter2Client.new.main
