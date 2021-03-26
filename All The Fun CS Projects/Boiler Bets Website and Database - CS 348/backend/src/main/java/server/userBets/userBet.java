package server.userBets;

import javax.persistence.*;
import javax.validation.constraints.*;
import org.springframework.data.jpa.domain.support.AuditingEntityListener;

public class userBet {
    private long userId;
    private long betId;
    private long amount;
    private Double payout;
    private String betSuccess;

    public userBet() {
        super();
    }

    public userBet(String userId, String betId, String amount, String payout, String betSuccess) {
        super();
        this.userId = Long.parseLong(userId);
        this.betId = Long.parseLong(betId);
        this.amount = Long.parseLong(amount);
        this.payout = Double.parseDouble(payout);
        this.betSuccess = betSuccess;
    }

    public long getUserId() {
        return this.userId;
    }

    public long getBetId() {
        return this.betId;
    }

    public long getAmount() {
        return this.amount;
    }

    public Double getPayout() {
        return this.payout;
    }

    public String getBetSuccess() {
        return this.betSuccess;
    }
}
